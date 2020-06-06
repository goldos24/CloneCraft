#pragma once

#include <cmath>
#include "player.h"
#include "maths.h"
#include "blocks.h"
#include "chunks.h"

namespace playerWorldInteraction
{

	maths::Vec3 getBlockPosInFrontOfPlayer(world::World world, player::Player player, float playerReach)
	{
		auto t = maths::positionFromRotation<float>(player.rotation) * playerReach;
		
		maths::Vec3 finalBlockPos;
		for (float i = 0.f; i < 100.f * playerReach; i++)
		{
			finalBlockPos = maths::Vec3(t.x, t.y, t.z) * i / 100.f / playerReach;
			auto blockID = world.getBlockID(finalBlockPos + player.position);
			if (blockID != blox::air)break;
		}

		return finalBlockPos + player.position;
	}

	void breakBlockInFrontOfPlayer(world::World world, player::Player player)
	{
		maths::Vec3 blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(world, player, 3);
		std::cout << "Block pos: " << blockPosInFrontOfPlayer.floor(1.f) << std::endl;
		world.setBlockID(blockPosInFrontOfPlayer, blox::air);
	}

	void setBlockInFrontOfPlayer(world::World world, player::Player player, float playerReach)
	{
		auto setBlockPos = getBlockPosInFrontOfPlayer(world, player, playerReach) + maths::positionFromRotation<float>(player.rotation) * -.5f;
		world.setBlockID(setBlockPos, blox::stone);
	}
}