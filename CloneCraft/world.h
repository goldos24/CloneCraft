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
			this-> chunks = new chunks::Chunk[size];
			for (int i = 0;i < this-> size; i++)
			{
				auto& chunk = chunks[i];
				chunk = chunks::initFlatChunk();
			}
		}
		int size;
		int chunkRenderDistance = 3;
		chunks::Chunk* chunks;


		auto chunkPosFromIndex(int i)
		{
			int x, y, z;
			maths::coord::indexToCoordinate(i, x, y, z, chunkRenderDistance);
			return maths::Vec3i(x, y, z);
		}

		void Render() //TODO replace
		{
			for (int i = 0; i < this->size; i++)
			{
				chunks[i].chunkPos = chunkPosFromIndex(i) * 16;
				chunks[i].Render();
			}
		}
	};
}