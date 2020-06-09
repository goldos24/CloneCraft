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

	void applyAcceleration(player::Player& player, float elapsedTime, maths::Vec3<float> acceleration)
	{
		player.movement += acceleration * elapsedTime;
	}

	void applyFriction(player::Player& player, float elapsedTime, float friction)
	{
		float frictionFactor = (1 < friction * elapsedTime) ? 0.f : 1 - friction * elapsedTime;
		player.movement = player.movement * frictionFactor;
	}

	bool isColliding(player::Player& player, world::World& world)
	{
		for (float i = -player.hitbox.x / 2.f; i < player.hitbox.x / 2.f; i++)
			for (float j = 0; j < player.hitbox.y; j++)
				for (float k = -player.hitbox.z / 2.f; k < player.hitbox.z / 2.f; k++)
				{
					if (world.getBlockID(player.position + maths::Vec3<float>(
						i > player.hitbox.x / 2.f ? player.hitbox.x / 2.f : i,
						-(j + 1 > player.hitbox.y ? player.hitbox.y : j),
						k > player.hitbox.z / 2.f ? player.hitbox.z / 2.f : k
					)))
						return true;
				}
		return false;
	}

	void clipMovement(player::Player& player, float elapsedTime, world::World& world)
	{
		auto appliedMovementVector = player.movement * elapsedTime;
		for (float j = 0; j < player.hitbox.y; j++)
		{
			if (world.getBlockID(player.position + maths::Vec3<float>(appliedMovementVector.x + player.movement.x > 0 ? 0.35 : -0.35, -j, 0.f)) != blox::air) player.movement.x = 0.f;
			// TODO fix bug with ze collision
			for (float i = -player.hitbox.x / 2.f; i < player.hitbox.x / 2.f; i += player.hitbox.x / 2.f)
				for (float k = -player.hitbox.z / 2.f; k < player.hitbox.z / 2.f; k += player.hitbox.z / 2.f)
					if (world.getBlockID(player.position + maths::Vec3<float>(i, appliedMovementVector.y + player.movement.y > 0 ? 0.f : -( j + 1 > player.hitbox.y ? player.hitbox.y : j ), k)) != blox::air) player.movement.y = 0.f;
			if (world.getBlockID(player.position + maths::Vec3<float>(0.f, -j, appliedMovementVector.z + player.movement.z > 0 ? 0.35 : -0.35)) != blox::air) player.movement.z = 0.f;
		}
	}
}