#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <cstdint>
#include "../world/chunkData.h"
#include "../world/chunks.h"
#include "../maths/maths.h"
#include "../files/files.h"
#include "../world/blockInfo.h"
#include "../entity/Entity.h"
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

		//void updateEntities(Game&, float elapsedTime);

		bool loadFromFile(std::string worldFileName);
		
		bool createWorld(std::string worldFileName, int seed);

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

		void updateEntities(Game& game, float elapsedTime);

		void unloadGarbageChunks();
#ifndef CLONECRAFT_NO_GFX
		void Render(texStorage::TextureAtlas& texAtlas, maths::Vec3<float>, maths::Vec3<float>);

		void RenderEntities(maths::Vec3<float>, maths::Vec3<float>);

		void markVisibleChunks(maths::Vec3<float> cameraRotation, maths::Vec3<float> cameraPosition);
#endif
		bool save();

		bool containsChunk(maths::Vec3<int> chunkPos);

		int& seed;

		private:

		// Only for state modifications corrupting the data
	};
}