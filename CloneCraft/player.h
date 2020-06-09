#pragma once

#include "maths.h"
#include "world.h"
#include "blocks.h"
#include "Entity.h"

namespace player
{
	const float ROTATION_X_BOUNDS = 89.f;

	struct Player : Entity
	{
		Player()
		{
			this->hitbox = maths::Vec3<float>(0.7f, 1.8f, 0.7f);
		}

		static float playerReach;

		void rotate(float X, float Y, float mouseSpeed)
		{
			this-> rotation.x += X * mouseSpeed;
			this-> rotation.y += Y * mouseSpeed;
			if (this->rotation.x < -ROTATION_X_BOUNDS) this->rotation.x = -ROTATION_X_BOUNDS;
			if (this->rotation.x > ROTATION_X_BOUNDS) this->rotation.x = ROTATION_X_BOUNDS;

			maths::capDegrees(this->rotation.y);
		}
	};
}

float player::Player::playerReach = 7.f;