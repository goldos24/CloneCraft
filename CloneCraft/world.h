#pragma once

#include <vector>
#include "chunks.h"
#include "maths.h"

namespace world
{
	struct World
	{
		World()
		{
			size = maths::cubeof(this->chunkRenderDistance);
			this->chunks = std::vector<chunks::Chunk>();
			for (int i = 0; i < size; i++)
			{
				int x, y, z;
				maths::coord::indexToCoordinate(i, x, y, z, this->chunkRenderDistance);
				this->chunks.push_back( chunks::initFlatChunk(maths::Vec3i(x * 16, y * 16, z * 16)) );
			}
		}
		int size;
		int chunkRenderDistance = 3;
		std::vector<chunks::Chunk> chunks;

		auto chunkPosFromIndex(int i)
		{
			int x, y, z;
			maths::coord::indexToCoordinate(i, x, y, z, chunkRenderDistance);
			return maths::Vec3i(x, y, z);
		}
		
		void moveTo(maths::Vec3i destination)
		{
			// Rounding the movement
			destination.x /= chunks::size;destination.y /= chunks::size;destination.z /= chunks::size;
			destination.x *= chunks::size;destination.y *= chunks::size;destination.z *= chunks::size;

			auto worldStart = maths::Vec3i(MAXINT, MAXINT, MAXINT);
			auto worldEnd = maths::Vec3i(MININT, MININT, MININT);

			for (auto chunk : this->chunks)
			{
				if (chunk.chunkPos.x < worldStart.x) worldStart.x = chunk.chunkPos.x;
				if (chunk.chunkPos.y < worldStart.y) worldStart.y = chunk.chunkPos.y;
				if (chunk.chunkPos.z < worldStart.z) worldStart.z = chunk.chunkPos.z;
			}

			worldEnd = worldStart + maths::Vec3i(this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size) ;

			auto destEnd = destination + maths::Vec3i(this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size);

			for (int i = destination.x; i < destEnd.x; i += chunks::size)
				for (int j = destination.y; j < destEnd.y; j += chunks::size)
					for (int k = destination.z; k < destEnd.z; k += chunks::size)
					{ 
						auto currentChunkPos = maths::Vec3i(i, j, k);
						if (!currentChunkPos.isInBounds(worldStart, worldEnd))
						{
							this->loadChunk(currentChunkPos);
						}
					}
		}

		void loadChunk(maths::Vec3i chunkPos)
		{
			this->chunks.push_back(
				chunks::initFlatChunk(chunkPos)
			);
		}

		void Render() //TODO replace
		{
			for (auto chunk : this->chunks)
			{
				chunk.Render();
			}
		}
	};
}