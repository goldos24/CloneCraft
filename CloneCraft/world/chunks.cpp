#include "chunks.h"
#include "../game/game.h"

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
	{
		//throw std::logic_error("Coordinate " + std::to_string(x)+ " " + std::to_string(y) + " " + std::to_string(z) + " out of Bounds!");
		return blox::air;
	}
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

constexpr int blockContainerSize = chunks::size + 1;

constexpr int blockContainerGetIndex(int x, int y, int z)
{
	return maths::squareof(blockContainerSize) * x + blockContainerSize * y + z;
}

void chunks::Chunk::calculateFaces(world::World& world)
{
	this->wereFacesCalculated = true;
	this->faceMutex.lock();
	blox::ID blockContainer[maths::cubeof(blockContainerSize)];
	renderData.clear();
	// Filling the block container with blocks from the chunk
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
			for(int k = 0; k < 16; ++k)
			{
				blockContainer[blockContainerGetIndex(i, j, k)] = chunk_getBlockUnsafely_macro(*this, i, j , k);
			}
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
		{
			blockContainer[blockContainerGetIndex(i, j, 16)] = world.getBlockID(maths::Vec3<int>(this->chunkPos.x + i, this->chunkPos.y + j, this->chunkPos.z + 16));
		}
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
		{
			blockContainer[blockContainerGetIndex(i, 16, j)] = world.getBlockID(maths::Vec3<int>(this->chunkPos.x + i, this->chunkPos.y + 16, this->chunkPos.z + j));
		}
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
		{
			blockContainer[blockContainerGetIndex(16, i, j)] = world.getBlockID(maths::Vec3<int>(this->chunkPos.x + 16, this->chunkPos.y + i, this->chunkPos.z + j));
		}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
			{
				//calculateAndPushBlock(i, j, k, newFaces);
				facePos::FacePosition positions[3];
				auto thisBlock = blockContainer[blockContainerGetIndex(i, j, k)];
				blox::ID ids[3] = { thisBlock, thisBlock, thisBlock };
				bool isThisBlockTransparent = blox::isTransparent(blockContainer[blockContainerGetIndex(i, j, k)]);
				bool opacityBools[3] =
				{
					blox::isTransparent(blockContainer[blockContainerGetIndex(i + 1, j, k)]),
					blox::isTransparent(blockContainer[blockContainerGetIndex(i, j + 1, k)]),
					blox::isTransparent(blockContainer[blockContainerGetIndex(i, j, k + 1)])
				};
				if (isThisBlockTransparent)
				{
					positions[0] = facePos::left;
					positions[1] = facePos::bottom;
					positions[2] = facePos::front;
					ids[0] = blockContainer[blockContainerGetIndex(i + 1, j, k)];
					ids[1] = blockContainer[blockContainerGetIndex(i, j + 1, k)];
					ids[2] = blockContainer[blockContainerGetIndex(i, j, k + 1)];
				}
				else
				{
					positions[0] = facePos::right;
					positions[1] = facePos::top;
					positions[2] = facePos::back;
				}
				const float lowX = (this->chunkPos.x + i);
				const float lowY = (this->chunkPos.y + j);
				const float lowZ = (this->chunkPos.z + k);
				const float highX = (this->chunkPos.x + i + 1);
				const float highY = (this->chunkPos.y + j + 1);
				const float highZ = (this->chunkPos.z + k + 1);

				if (isThisBlockTransparent ^ opacityBools[0])
				{
					this->renderData.push_back(renderData::makeFace(ids[0], highX, lowY, lowZ, false, positions[0]));
				}

				if (isThisBlockTransparent ^ opacityBools[1])
				{
					this->renderData.push_back(renderData::makeFace(ids[1], lowX, highY, lowZ, false, positions[1]));
				}

				if (isThisBlockTransparent ^ opacityBools[2])
				{
					this->renderData.push_back(renderData::makeFace(ids[2], lowX, lowY, highZ, false, positions[2]));
				}
			}
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
					if(!chunks::isCoordinateInBounds(localBlockPos.x, localBlockPos.y, localBlockPos.z))
						continue;
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

	if (chunkPos.y >= 32 * getVoxelSubdivision())
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
				float height = heightMap[i * 16 + k] * getVoxelSubdivision();
				chunk->setBlockUnsafely(
					(height < float(j + chunkPos.y - 12 * getVoxelSubdivision()) ? blox::air :
						height < float(j + chunkPos.y - 11 * getVoxelSubdivision()) ? surface :
						height < float(j + chunkPos.y - 8 * getVoxelSubdivision()) ? blox::dirt : blox::stone),
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
