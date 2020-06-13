#pragma once


#include "../maths/maths.h"
#include "../world/world.h"
#include "../world/blocks.h"
#include "../entity/Entity.h"

namespace player
{
	const float ROTATION_X_BOUNDS = 89.f;

	struct Player : Entity
	{
		Player();

		static float playerReach;

		void rotate(float X, float Y, float mouseSpeed);
	};
}
