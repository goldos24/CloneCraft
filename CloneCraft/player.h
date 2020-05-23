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
			this-> rotation.y += X * mouseSpeed;
			this-> rotation.x += Y * mouseSpeed;
		}
	};

}