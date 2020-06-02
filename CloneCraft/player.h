#pragma once

#include "maths.h"

namespace player
{
	const float ROTATION_Y_BOUND = 89.f;

	struct Player
	{
		Player()
		{}

		maths::Vec3 position;
		maths::Vec3 rotation;

		void rotate(float X, float Y, float mouseSpeed)
		{
			this-> rotation.x += X * mouseSpeed;
			this-> rotation.y += Y * mouseSpeed;
			if (this->rotation.x < -ROTATION_Y_BOUND) this->rotation.x = -ROTATION_Y_BOUND;
			if (this->rotation.x > ROTATION_Y_BOUND) this->rotation.x = ROTATION_Y_BOUND;

			maths::capDegrees(this->rotation.y);
		}
	};

}