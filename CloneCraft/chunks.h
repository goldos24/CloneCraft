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
		return maths::is3dCoordInRange(x, y, z, 0, size);
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
			bool isSelectedBlockTransparent = blox::isTransparent(this->getBlock(x, y, z));

			calculateAndPushFace(x, y, z, maths::unitVectors::down, isSelectedBlockTransparent, facePos::bottom);
			calculateAndPushFace(x, y, z, maths::unitVectors::back, isSelectedBlockTransparent, facePos::back);
			calculateAndPushFace(x, y, z, maths::unitVectors::right, isSelectedBlockTransparent, facePos::right);
		}

		void calculateAndPushFace(int x, int y, int z, maths::Vec3i off,
			bool isSelectedBlockTransparent,
			facePos::FacePosition swappedFacePosition)
		{
			bool swapSides = !isSelectedBlockTransparent;
			bool isOffsetBlockTransparent = blox::isTransparent(this->getBlock(x + off.x, y + off.y, z + off.z));

			if (isSelectedBlockTransparent ^ isOffsetBlockTransparent)
				this->renderData.push_back(
					calculateFace(x, y, z, off, swapSides, facePos::swap(swappedFacePosition), swappedFacePosition));
		}

		renderData::BlockFace calculateFace(int x, int y, int z, maths::Vec3i off,
			bool swapSides,
			facePos::FacePosition facePosition, facePos::FacePosition swappedFacePosition)
		{
			return renderData::makeFace(
				(swapSides) ? 
				this->getBlock(x, y, z)
				: 
				this->getBlock(x + off.x, y + off.y, z + off.z), 
				x + this->chunkPos.x, y + this->chunkPos.y, z + this->chunkPos.z, swapSides, facePosition, swappedFacePosition);
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