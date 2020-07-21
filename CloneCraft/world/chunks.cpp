#include "chunks.h"

int chunks::coordinateToIndex(int x, int y, int z)
{
	return maths::coord::coordinateToIndex(x, y, z, size);
}

void chunks::indexToCoordinate(int i, int& x, int& y, int& z)
{
	maths::coord::indexToCoordinate(i, x, y, z, size);
}

bool chunks::isCoordinateInBounds(int x, int y, int z)
{
	return maths::is3dCoordInRange(x, y, z, 0, size);
}

chunks::Chunk::Chunk()
{
	//this->calculateFaces();
}

chunks::Chunk::~Chunk()
{

}

blox::ID chunks::Chunk::getBlock(int x, int y, int z)
{
	if (!isCoordinateInBounds(x, y, z))
		return blox::air;
	return this->blocks[coordinateToIndex(x, y, z)];
}

void chunks::Chunk::setBlock(blox::ID id, int x, int y, int z)
{
	if (!isCoordinateInBounds(x, y, z))
		return;
	this->blocks[coordinateToIndex(x, y, z)] = id;
}

void chunks::Chunk::setBlockUnsafely(blox::ID id, int x, int y, int z)
{
	this->blocks[coordinateToIndex(x, y, z)] = id;
}

void chunks::Chunk::placeBlock(blox::ID id, int x, int y, int z)
{
	this->setBlock(id, x, y, z);
#ifndef CLONECRAFT_NO_GFX
	this->calculateFaces();
#endif
}

#ifndef CLONECRAFT_NO_GFX
void chunks::Chunk::calculateAndPushBlock(int x, int y, int z, std::vector<renderData::BlockFace>& renderDataVector)
{
	bool isSelectedBlockTransparent = blox::isTransparent(this->getBlock(x, y, z));
	bool swapSides = !isSelectedBlockTransparent;

	int aX = x + this->chunkPos.x;
	int aY = y + this->chunkPos.y;
	int aZ = z + this->chunkPos.z;

	calculateAndPushFace(x, y, z, aX, aY, aZ, maths::unitVectors::down, isSelectedBlockTransparent, swapSides, facePos::top, renderDataVector);
	calculateAndPushFace(x, y, z, aX, aY, aZ, maths::unitVectors::back, isSelectedBlockTransparent, swapSides, facePos::front, renderDataVector);
	calculateAndPushFace(x, y, z, aX, aY, aZ, maths::unitVectors::right, isSelectedBlockTransparent, swapSides, facePos::left, renderDataVector);
}

void chunks::Chunk::calculateAndPushFace(int x, int y, int z, int actualX, int actualY, int actualZ,
	maths::Vec3<int> offset,
	bool isSelectedBlockTransparent, bool swapSides,
	facePos::FacePosition swappedFacePosition, std::vector<renderData::BlockFace>& renderDataVector)
{
	bool isOffsetBlockTransparent = blox::isTransparent(this->getBlock(x + offset.x, y + offset.y, z + offset.z));

	if (isSelectedBlockTransparent ^ isOffsetBlockTransparent)
		renderDataVector.push_back(
			calculateFace(x, y, z, actualX, actualY, actualZ,
				offset, swapSides, swappedFacePosition, renderDataVector));
}

renderData::BlockFace chunks::Chunk::calculateFace(int x, int y, int z, int actualX, int actualY, int actualZ,
	maths::Vec3<int> offset,
	bool swapSides,
	facePos::FacePosition facePosition, std::vector<renderData::BlockFace>& renderData)
{
	return renderData::makeFace(
		(swapSides) ?
		this->getBlock(x, y, z)
		:
		this->getBlock(x + offset.x, y + offset.y, z + offset.z),
		actualX, actualY, actualZ, swapSides, facePosition);
}

void chunks::Chunk::calculateFaces()
{
	std::vector<renderData::BlockFace> newFaces;
	this->renderData.clear();
	for (int i = 0; i <= size; i++)
		for (int j = 0; j <= size; j++)
			for (int k = 0; k <= size; k++)
			{
				calculateAndPushBlock(i, j, k, newFaces);
			}
	this->faceMutex.lock();
	std::swap<renderData::BlockFace>(this->renderData, newFaces);
	this->faceMutex.unlock();
}

void chunks::Chunk::Render(texStorage::TextureAtlas& texAtlas) //TODO replace loop
{
	this->faceMutex.lock();
	for (auto face : renderData)
		face.render(texAtlas);
	this->faceMutex.unlock();
}
#endif

std::shared_ptr<chunks::Chunk> chunks::initFlatChunk(maths::Vec3<int> chunkPos)
{
	std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
	chunk->chunkPos = chunkPos;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
			{
				chunk->setBlock(
					(j > 1 ? blox::air : blox::stone),
					i, j, k
				);
			}
	chunk->setBlock(blox::grass, 8, 14, 8);
#ifndef CLONECRAFT_NO_GFX
	chunk->calculateFaces();
#endif
	return chunk;
}

namespace
{
	void spawnTreeInChunk(chunks::Chunk& chunk, maths::Vec3<float> treePosition) 
	{
		maths::Vec3<int> localTreePos(treePosition.x - chunk.chunkPos.x, treePosition.y - chunk.chunkPos.y, treePosition.z - chunk.chunkPos.z);
		for (int i = 0; i < 5; ++i)
		{
			maths::Vec3<int> localBlockPos = localTreePos + maths::Vec3<int>(0, i, 0);
			chunk.setBlock(blox::wewd, localBlockPos.x, localBlockPos.y, localBlockPos.z);
		}
		
		for (int i = -2; i < 3; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = -2; k < 3; ++k)
				{
					maths::Vec3<int> localBlockPos = localTreePos + maths::Vec3<int>(i, j + 4, k);
					if (chunk.getBlock(localBlockPos.x, localBlockPos.y, localBlockPos.z) == blox::air)
					{
						chunk.setBlock(blox::leaves, localBlockPos.x, localBlockPos.y, localBlockPos.z);
					}
				}
	}
}

std::shared_ptr<chunks::Chunk> chunks::initNormalChunk(maths::Vec3<int> chunkPos, float seed)
{
	std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
	chunk->chunkPos = chunkPos;

	if (chunkPos.y < -16 || chunkPos.y >= 32)
	{
		for (blox::ID& id : chunk->blocks)
		{
			id = blox::air;
		}
		chunk->calculateFaces();
		return chunk;
	}

	// \\
	Hills and mountains

	auto heightMap = terrainGen::createHeightMap(maths::Vec2<float>(float(chunkPos.x), float(chunkPos.z)), seed);

	blox::ID surface = chunkPos.y >= 0 ? blox::grass : blox::dirt;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
			{
				float height = heightMap[i * 16 + k];
				chunk->setBlockUnsafely(
					(height < float(j + chunkPos.y - 12) ? blox::air :
						height < float(j + chunkPos.y - 11) ? surface :
						height < float(j + chunkPos.y - 8) ? blox::dirt : blox::stone),
					i, j, k
				);
			}
	// \\
	Trees

	std::vector<maths::Vec3<float>> treePositions;

	for(int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
			terrainGen::getTreePositions(maths::Vec2<float>(float(chunkPos.x + i * chunks::size), float(chunkPos.z + j * chunks::size)), treePositions, seed);

	for (auto treePos : treePositions)
	{
		if(treePos.y > 0)
		spawnTreeInChunk(*chunk, treePos);
	}

#ifndef CLONECRAFT_NO_GFX
	chunk->calculateFaces();
#endif
	return chunk;
}

void chunks::Chunk::renderEntities(maths::Vec3<float> cameraPosition, maths::Vec3<float> cameraRotation)
{
	for (auto entity : this->entities)
	{
		entity->render(cameraPosition, cameraRotation);
	}
}

void chunks::Chunk::updateEntities(Game& game, float elapsedTime)
{
	for (auto entity : this->entities)
		entity->update(game, elapsedTime);
}