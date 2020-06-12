#pragma once

#include "../world/world.h"
#include "../player/player.h"
#include "../maths/maths.h"
#include "../world/blocks.h"

namespace playerControls
{
	void moveUp(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.position.y += movementSpeed * elapsedTime;
	}

	void moveDown(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.position.y -= movementSpeed * elapsedTime;
	}

	void moveRight(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.applyAcceleration(elapsedTime, maths::Vec3<float>(maths::cosd(-player.rotation.y) * movementSpeed, 0.f, -maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveLeft(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.applyAcceleration(elapsedTime, maths::Vec3<float>(-maths::cosd(-player.rotation.y) * movementSpeed, 0.f, maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveForward(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.applyAcceleration(elapsedTime, maths::Vec3<float>(-maths::sind(-player.rotation.y) * movementSpeed, 0.f, -maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void moveBackward(player::Player& player, float elapsedTime, float movementSpeed)
	{
		player.applyAcceleration(elapsedTime, maths::Vec3<float>(maths::sind(-player.rotation.y) * movementSpeed, 0.f, maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void jump(player::Player& player, float height, world::World& world)
	{
		if(player.isStandingOnASurface(world))
		player.movement.y = height;
	}

	void applyGravity(player::Player& player, float elapsedTime, float gravity)
	{
		player.applyAcceleration(elapsedTime, maths::Vec3<float>(0.f, -1.f, 0.f) * gravity);
	}
}