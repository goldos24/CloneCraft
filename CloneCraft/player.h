#pragma once

#include "maths.h"
#include "world.h"
#include "blocks.h"

namespace player
{
	const float ROTATION_Y_BOUND = 89.f;

	struct Player
	{
		Player()
		{}

		static float playerReach;

		maths::Vec3 position, rotation, movement, hitbox = maths::Vec3(0.7f, 1.8f, 0.7f);

		void rotate(float X, float Y, float mouseSpeed)
		{
			this-> rotation.x += X * mouseSpeed;
			this-> rotation.y += Y * mouseSpeed;
			if (this->rotation.x < -ROTATION_Y_BOUND) this->rotation.x = -ROTATION_Y_BOUND;
			if (this->rotation.x > ROTATION_Y_BOUND) this->rotation.x = ROTATION_Y_BOUND;

			maths::capDegrees(this->rotation.y);
		}

		bool isStandingOnASurface(world::World& world)
		{
			for (float i = -this->hitbox.x / 2.f; i < this->hitbox.x / 2.f; i += this->hitbox.x / 2.f)
				for (float k = -this->hitbox.z / 2.f; k < this->hitbox.z / 2.f; k += this->hitbox.z / 2.f)
				{
					if (world.getBlockID(this->position + maths::Vec3(i, - this->hitbox.y, k)) != blox::air) return true;
				}
			return false;
		}
	};
}

float player::Player::playerReach = 5.f;