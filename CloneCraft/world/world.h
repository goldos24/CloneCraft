#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <cstdint>
#include "../world/chunkData.h"
#include "../world/chunks.h"
#include "../maths/maths.h"
#include "../files/files.h"
#include "../world/blockInfo.h"
#ifndef CLONECRAFT_NO_GFX
#include "../glu/oldFunctions.h"
#endif

namespace world
{
	struct World
	{
		World();
		std::string worldFileName;
		maths::Vec3<int> worldPos = maths::Vec3<int>(0, 0, 0);
		int size;
		int chunkRenderDistance = 11;
		std::map<uint64_t, std::shared_ptr<chunks::Chunk>> chunks;
		saveData::Manager mgr;

		bool loadFromFile(std::string worldFileName);
		
		bool createWorld(std::string worldFileName);

		void unload();

		void moveTo(maths::Vec3<int> destination);

		std::shared_ptr<chunks::Chunk> getChunk(maths::Vec3<int> chunkPos);

		blox::ID getBlockID(maths::Vec3<int> blockPos);

		blox::ID getBlockID(maths::Vec3<float> blockPosF);

		void setBlockID(maths::Vec3<int> blockPos, blox::ID id);

		void setBlockID(maths::Vec3<float> blockPosF, blox::ID id);

		std::shared_ptr<chunks::Chunk> findChunkFromPlayerPosition(maths::Vec3<float> playerPosition);

		maths::Vec3<float> getPlayerPositionInsideCurrentChunk(maths::Vec3<float> playerPosition);

		void loadChunk(maths::Vec3<int> chunkPos);

		void unloadGarbageChunks();
#ifndef CLONECRAFT_NO_GFX
		void Render(texStorage::TextureAtlas& texAtlas);

		void markVisibleChunks(maths::Vec3<float> cameraRotation);
#endif
		bool save();

		bool containsChunk(maths::Vec3<int> chunkPos);

		private:

		// Only for state modifications corrupting the data
	};
}