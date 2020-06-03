#pragma once

#include "maths.h"
#include "player.h"

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
}