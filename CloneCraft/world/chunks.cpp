#include "chunks.h"
#include "../game/GAME.H"

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


blox::ID chunks::Chunk::getBlock(int x, int y, int z, world::World& world)
{
	if (!isCoordinateInBounds(x, y, z))
		return world.getBlockID(this->chunkPos + maths::Vec3<int>(x, y, z));
	return this->blocks[coordinateToIndex(x, y, z)];
}

#define chunk_getBlockUnsafely_macro(chunk, x, y, z)  ((chunk).blocks[coordinateToIndex(x, y, z)])

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

void chunks::Chunk::placeBlock(blox::ID id, int x, int y, int z, world::World& world)
{
	this->setBlock(id, x, y, z);
#ifndef CLONECRAFT_NO_GFX
	this->calculateFaces(world);
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

static int faceCount = 0;

void chunks::Chunk::calculateFaces(world::World& world)
{
	this->wereFacesCalculated = true;
	std::vector<renderData::BlockFace> newFaces;
	this->renderData.clear();
	for (int i = -1; i < size; i++)
		for (int j = -1; j < size; j++)
			for (int k = -1; k < size; k++)
			{
				//calculateAndPushBlock(i, j, k, newFaces);
				facePos::FacePosition positions[3];
				auto thisBlock = this->getBlock(i, j, k, world);
				blox::ID ids[3] = { thisBlock, thisBlock, thisBlock };
				bool isThisBlockTransparent = blox::isTransparent(this->getBlock(i, j, k, world));
				bool opacityBools[3] =
				{
					blox::isTransparent(i < size - 1 && i >= 0 && j >= 0 && k >= 0 ? chunk_getBlockUnsafely_macro(*this, i + 1, j, k) : this->getBlock(i + 1, j, k, world)),
					blox::isTransparent(j < size - 1 && i >= 0 && j >= 0 && k >= 0 ? chunk_getBlockUnsafely_macro(*this, i, j + 1, k) : this->getBlock(i, j + 1, k, world)),
					blox::isTransparent(k < size - 1 && i >= 0 && j >= 0 && k >= 0 ? chunk_getBlockUnsafely_macro(*this, i, j, k + 1) : this->getBlock(i, j, k + 1, world))
				};
				if (isThisBlockTransparent)
				{
					positions[0] = facePos::left;
					positions[1] = facePos::bottom;
					positions[2] = facePos::front;
					ids[0] = this->getBlock(i + 1, j, k, world);
					ids[1] = this->getBlock(i, j + 1, k, world);
					ids[2] = this->getBlock(i, j, k + 1, world);
				}
				else
				{
					positions[0] = facePos::right;
					positions[1] = facePos::top;
					positions[2] = facePos::back;
				}

				if (isThisBlockTransparent ^ opacityBools[0])
				{
					newFaces.push_back(renderData::makeFace(ids[0], this->chunkPos.x + i + 1, this->chunkPos.y + j, this->chunkPos.z + k, false, positions[0]));
				}

				if (isThisBlockTransparent ^ opacityBools[1])
				{
					newFaces.push_back(renderData::makeFace(ids[1], this->chunkPos.x + i, this->chunkPos.y + j + 1, this->chunkPos.z + k, false, positions[1]));
				}

				if (isThisBlockTransparent ^ opacityBools[2])
				{
					newFaces.push_back(renderData::makeFace(ids[2], this->chunkPos.x + i, this->chunkPos.y + j, this->chunkPos.z + k + 1, false, positions[2]));
				}
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

	if (chunkPos.y >= 32)
	{
		for (blox::ID& id : chunk->blocks)
		{
			id = blox::air;
		}
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

#ifndef CLONECRAFT_NO_GFX//\
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
	std::vector<int> entitiesToBeRemoved;
	int offset = 0;

	for (int i = 0; i < this->entities.size(); ++i)
	{
		auto entity = entities[i];
		entity->update(game, elapsedTime);
		if (!(entity->position - maths::convertVec3<int, float>(this->chunkPos)).isInBounds({ 0,0,0 }, { 16,16,16 }))
		{
			entitiesToBeRemoved.push_back(i);
		}
	}

	for (auto i : entitiesToBeRemoved)
	{
		auto entity = entities[i];
		this->entities.erase(this->entities.begin() + i - offset);
		offset++;
		auto newEntityChunkPos = chunks::convertToChunkPos(entity->position);
		game.gameWorld.getChunk(newEntityChunkPos)->entities.push_back(entity);
	}
}