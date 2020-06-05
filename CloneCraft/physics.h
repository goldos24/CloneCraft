#pragma once

#include "maths.h"
#include "player.h"
#include "world.h"
#include "blocks.h"

namespace physixx
{
	void applyMovement(player::Player& player, float elapsedTime)
	{
		player.position += player.movement * elapsedTime;
	}

	void applyAcceleration(player::Player& player, float elapsedTime, maths::Vec3 acceleration)
	{
		player.movement += acceleration * elapsedTime;
	}

	void applyFriction(player::Player& player, float elapsedTime, float friction)
	{
		float frictionFactor = (1 < friction * elapsedTime) ? 0.f : 1 - friction * elapsedTime;
		player.movement = player.movement * frictionFactor;
	}
#include <iostream>
	void clipMovement(player::Player& player, float elapsedTime, world::World& world)
	{
		auto newPosition = player.position + player.movement * elapsedTime;
		std::cout << maths::convertVec3<float, int>(newPosition + maths::Vec3(0.f, 0.f, 0.f)) << "\r";
		if (world.getBlockID(maths::convertVec3<float, int>(newPosition + maths::Vec3(0.f, 0.f, 0.f))) != blox::air) player.movement = maths::Vec3(0.f, 0.f, 0.f);
	}
}