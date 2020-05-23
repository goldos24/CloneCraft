#pragma once

#include "maths.h"

namespace player
{

	struct Player
	{
		Player()
		{}

		maths::Vec3 position;
		maths::Vec3 rotation;

		auto rotate(float X, float Y, float mouseSpeed)
		{
			this-> rotation.x += X * mouseSpeed;
			this-> rotation.y += Y * mouseSpeed;
			if (this->rotation.x < -90.f) this->rotation.x = -90.f;
			if (this->rotation.x > 90.f) this->rotation.x = 90.f;
		}
	};

}