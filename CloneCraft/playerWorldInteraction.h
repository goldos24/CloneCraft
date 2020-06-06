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
		int fixedRotationX = player.rotation.x + 90;
		int rightAngleMinusFixedRotationX = 90 - fixedRotationX;
		int rightAngleMinusRotationY = 90 - player.rotation.y;

		float tX = maths::cosd(rightAngleMinusRotationY) * playerReach * abs(maths::cosd(rightAngleMinusFixedRotationX));
		float tY = maths::sind(rightAngleMinusFixedRotationX) * playerReach;
		float tZ = maths::sind(-rightAngleMinusRotationY) * playerReach * abs(maths::cosd(rightAngleMinusFixedRotationX));
		
		maths::Vec3 finalBlockPos;
		for (float i = 0.f; i < 100.f * playerReach; i++)
		{
			finalBlockPos = maths::Vec3(tX, tY, tZ) * i / 100.f / playerReach;
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
}