#pragma once

#include <cmath>
#include "player.h"
#include "maths.h"
#include "blocks.h"
#include "chunks.h"
#include "physics.h"

namespace playerWorldInteraction
{

	maths::Vec3 getBlockPosInFrontOfPlayer(world::World& world, player::Player& player)
	{
		auto t = maths::positionFromRotation<float>(player.rotation) * player.playerReach;
		
		maths::Vec3 finalBlockPos;
		for (float i = 0.f; i < 100.f * player.playerReach; i++)
		{
			finalBlockPos = maths::Vec3(t.x, t.y, t.z) * i / 100.f / player.playerReach;
			auto blockID = world.getBlockID(finalBlockPos + player.position);
			if (blockID != blox::air)break;
		}

		return finalBlockPos + player.position;
	}

	void breakBlockInFrontOfPlayer(world::World& world, player::Player& player)
	{
		maths::Vec3 blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(world, player);
		std::cout << "Block pos: " << blockPosInFrontOfPlayer.floor(1.f) << std::endl;
		world.setBlockID(blockPosInFrontOfPlayer, blox::air);
	}

	void setBlockInFrontOfPlayer(world::World& world, player::Player& player)
	{
		auto oldBlockPos = getBlockPosInFrontOfPlayer(world, player);
		if (world.getBlockID(oldBlockPos) == blox::air) return;
		auto newBlockPos = oldBlockPos + maths::positionFromRotation<float>(player.rotation) * -.5f;
		world.setBlockID(newBlockPos, blox::stone);
		if(physixx::isColliding(player, world)) world.setBlockID(newBlockPos, blox::air);
	}
}