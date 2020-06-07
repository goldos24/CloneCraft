#pragma once

#include "maths.h"

namespace player
{
	const float ROTATION_X_BOUNDS = 89.f;

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
			if (this->rotation.x < -ROTATION_X_BOUNDS) this->rotation.x = -ROTATION_X_BOUNDS;
			if (this->rotation.x > ROTATION_X_BOUNDS) this->rotation.x = ROTATION_X_BOUNDS;

			maths::capDegrees(this->rotation.y);
		}
	};
}

float player::Player::playerReach = 7.f;