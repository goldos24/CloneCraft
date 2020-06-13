#pragma once

#include "../world/world.h"
#include "../player/player.h"
#include "../maths/maths.h"
#include "../world/blocks.h"

namespace playerControls
{
	void moveUp(player::Player& player, float elapsedTime, float movementSpeed);

	void moveDown(player::Player& player, float elapsedTime, float movementSpeed);

	void moveRight(player::Player& player, float elapsedTime, float movementSpeed);

	void moveLeft(player::Player& player, float elapsedTime, float movementSpeed);

	void moveForward(player::Player& player, float elapsedTime, float movementSpeed);

	void moveBackward(player::Player& player, float elapsedTime, float movementSpeed);

	void jump(player::Player& player, float height, world::World& world);

	void applyGravity(player::Player& player, float elapsedTime, float gravity);
}