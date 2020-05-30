#pragma once

#include <iostream>
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
				this->loadChunk(maths::Vec3i(x * 16, y * 16, z * 16));
			}
		}

		maths::Vec3i worldPos = maths::Vec3i(0, 0, 0);
		int size;
		int chunkRenderDistance = 5;
		std::vector<chunks::Chunk> chunks;

		auto chunkPosFromIndex(int i)
		{
			int x, y, z;
			maths::coord::indexToCoordinate(i, x, y, z, chunkRenderDistance);
			return maths::Vec3i(x, y, z);
		}

		void cleanupChunkVector()
		{
			auto newChunks = std::vector<chunks::Chunk>(maths::cubeof(this->chunkRenderDistance));
			auto chunkExistenceData = std::vector<bool>(newChunks.size()); for (int i = 0; i < chunkExistenceData.size(); i++) chunkExistenceData[i] = false;


			// Copying existing chunks
			for (auto chunk : this->chunks)
			{
				if (chunk.chunkPos.isInBounds(worldPos, maths::Vec3i(worldPos.x + chunkRenderDistance * chunks::size, worldPos.y + chunkRenderDistance * chunks::size, worldPos.z + chunkRenderDistance * chunks::size)))
				{
					auto chunkArrayPosition = maths::Vec3i((chunk.chunkPos.x - this->worldPos.x) / chunks::size, (chunk.chunkPos.y - this->worldPos.y) / chunks::size, (chunk.chunkPos.z - this->worldPos.z) / chunks::size);
					auto chunkIndex = maths::coord::coordinateToIndex(chunkArrayPosition.x, chunkArrayPosition.y, chunkArrayPosition.z, this->chunkRenderDistance);

					//std::cout << chunkArrayPosition.toString() << " " << chunk.chunkPos.toString() << " " << worldPos.toString() << " " << maths::Vec3i(worldPos.x + chunkRenderDistance * chunks::size, worldPos.y + chunkRenderDistance * chunks::size, worldPos.z + chunkRenderDistance * chunks::size).toString() << " " << chunkIndex << std::endl;
					newChunks[chunkIndex] = chunk;
					chunkExistenceData[chunkIndex] = true;
				}
			}

			// Loading missing chunks

			for (int i = 0; i < chunkExistenceData.size(); i++)
			{
				if (!chunkExistenceData[i])
				{
					int x, y, z;
					maths::coord::indexToCoordinate(i, x, y, z, this->chunkRenderDistance);

					newChunks[i] = getChunk(this->worldPos + maths::Vec3i(x, y, z) * chunks::size);
				}
			}

			this->chunks = newChunks;
		}
		
		void moveTo(maths::Vec3i destination)
		{
			// Rounding the movement
			destination.x /= chunks::size;destination.y /= chunks::size;destination.z /= chunks::size;
			destination.x *= chunks::size;destination.y *= chunks::size;destination.z *= chunks::size;

			auto worldStart = this->worldPos;
			auto worldEnd = maths::Vec3i(MININT, MININT, MININT);

			worldEnd = worldStart + maths::Vec3i(this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size) ;

			auto destEnd = destination + maths::Vec3i(this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size, this->chunkRenderDistance * chunks::size);

			// Deleting chunks too far away
			for (int i = 0; i < this->chunks.size(); i++)
			{
				if (!chunks[i].chunkPos.isInBounds(destination, destEnd))
				{
					this->unloadChunk(i);
				}
			}

			// Loading new Chunks
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
			
			this->worldPos = destination;
			this->cleanupChunkVector();
		}

		chunks::Chunk getChunk(maths::Vec3i chunkPos)
		{
			return chunks::initNormalChunk(chunkPos);
		}

		void loadChunk(maths::Vec3i chunkPos)
		{
			this->chunks.push_back(
				this->getChunk(chunkPos)
			);
		}

		void unloadChunk(int index)
		{
			this->chunks.erase(this->chunks.begin() + index);
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