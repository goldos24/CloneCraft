#pragma once
#include "blocks.h"
#include "renderer.h"
#include "maths.h"
#include "renderData.h"
#include <vector>
#include "terrainGenerator.h"

namespace chunks 
{

	const int size = 16;

	auto coordinateToIndex(int x, int y, int z) -> int
	{
		return maths::coord::coordinateToIndex(x, y, z, size);
	}

	auto indexToCoordinate(int i, int& x, int& y, int& z)
	{
		maths::coord::indexToCoordinate(i, x, y, z, size);
	}

	auto isCoordinateInBounds(int x, int y, int z)
	{
		return (

			x >= 0 && x < size && // Better Code may be possible
			y >= 0 && y < size &&
			z >= 0 && z < size

			);
	}

	struct Chunk
	{
		Chunk()
		{
			//this->calculateFaces();
		}

		~Chunk()
		{
			
		}

		std::vector<renderData::BlockFace> renderData = std::vector<renderData::BlockFace>();

		maths::Vec3i chunkPos;

		blox::ID blocks[maths::cubeof(size)];



		auto getBlock(int x, int y, int z)
		{
			if ( ! isCoordinateInBounds(x, y, z) )
				return blox::air;
			return this-> blocks[ coordinateToIndex(x, y, z) ];
		}

		auto setBlock(blox::ID id, int x, int y, int z)
		{
			if (!isCoordinateInBounds(x, y, z))
				return;
			this-> blocks[coordinateToIndex(x, y, z)] = id;
		}

		auto calculateAndPushBlock(int x, int y, int z)
		{
			int absoluteX = x + this->chunkPos.x;
			int absoluteY = y + this->chunkPos.y;
			int absoluteZ = z + this->chunkPos.z;

			bool isSelectedBlockTransparent = blox::isTransparent(this->getBlock(x, y, z));
			bool swapSides = !isSelectedBlockTransparent;
			if (isSelectedBlockTransparent ^
				blox::isTransparent(this->getBlock(x, y - 1, z)))
				this->renderData.push_back(
				renderData::makeBottomFace(
					(swapSides) ?
					this->getBlock(x, y, z)
					:
					this->getBlock(x, y - 1, z)
					, absoluteX, absoluteY, absoluteZ, swapSides));

			if (isSelectedBlockTransparent ^
				blox::isTransparent(this->getBlock(x, y, z - 1)))
				this->renderData.push_back(
				renderData::makeFrontFace(
					(swapSides) ?
					this->getBlock(x, y, z)
					:
					this->getBlock(x, y, z - 1)
					, absoluteX, absoluteY, absoluteZ, swapSides));

			if (isSelectedBlockTransparent ^
				blox::isTransparent(this->getBlock(x - 1, y, z)))
				this->renderData.push_back(
				renderData::makeLeftFace(
					(swapSides) ?
					this->getBlock(x, y, z)
					:
					this->getBlock(x - 1, y, z)
					, absoluteX, absoluteY, absoluteZ, swapSides));
		}

		void calculateFaces()
		{
			this->renderData.clear();
			for (int i = 0; i <= size; i++)
				for (int j = 0; j <= size; j++)
					for (int k = 0; k <= size; k++)
					{
						calculateAndPushBlock(i, j, k);
					}
		}

		auto Render() //TODO replace loop
		{
			for (auto face : renderData)
				face.render();
		}

	};

	auto initFlatChunk(maths::Vec3i chunkPos)
	{
		Chunk chunk;
		chunk.chunkPos = chunkPos;
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				for (int k = 0; k < size; k++)
				{
					chunk.setBlock(
						(j > 1  ? blox::air : blox::stone),
						i, j, k
					);
				}
		chunk.setBlock(blox::grass, 8, 14, 8);
		chunk.calculateFaces();
		return chunk;
	}

	auto initNormalChunk(maths::Vec3i chunkPos)
	{
		Chunk chunk;
		chunk.chunkPos = chunkPos;

		if (chunkPos.y < 0 || chunkPos.y >= 16)
		{
			for (blox::ID& id : chunk.blocks)
			{
				id = blox::air;
			}
			chunk.calculateFaces();
			return chunk;
		}

		auto heightMap = terrainGen::createHeightMap( maths::Vec2<float>( float(chunkPos.x), float(chunkPos.z)) );

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				for (int k = 0; k < size; k++)
				{
					chunk.setBlock(
						(heightMap[i * 16 + k] < float(j - 8) ? blox::air : blox::grass),
						i, j, k
					);
				}
		chunk.calculateFaces();
		return chunk;
	}
}