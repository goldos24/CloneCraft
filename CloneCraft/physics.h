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
	void clipMovement(player::Player& player, float elapsedTime, world::World& world)
	{
		auto appliedMovementVector = player.movement * elapsedTime;
		if (world.getBlockID(player.position + maths::Vec3(appliedMovementVector.x, 0.f, 0.f)) != blox::air) player.movement.x = 0.f;
		if (world.getBlockID(player.position + maths::Vec3(0.f, appliedMovementVector.y, 0.f)) != blox::air) player.movement.y = 0.f;
		if (world.getBlockID(player.position + maths::Vec3(0.f , 0.f, appliedMovementVector.z)) != blox::air) player.movement.z = 0.f;
	}
}