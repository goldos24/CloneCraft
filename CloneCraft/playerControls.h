#pragma once

#include "world.h"
#include "player.h"
#include "physics.h"
#include "maths.h"
#include "blocks.h"

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
		physixx::applyAcceleration(player, elapsedTime, maths::Vec3(maths::cosd(-player.rotation.y) * movementSpeed, 0.f, -maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveLeft(player::Player& player, float elapsedTime, float movementSpeed)
	{
		physixx::applyAcceleration(player, elapsedTime, maths::Vec3(-maths::cosd(-player.rotation.y) * movementSpeed, 0.f, maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveForward(player::Player& player, float elapsedTime, float movementSpeed)
	{
		physixx::applyAcceleration(player, elapsedTime, maths::Vec3(-maths::sind(-player.rotation.y) * movementSpeed, 0.f, -maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void moveBackward(player::Player& player, float elapsedTime, float movementSpeed)
	{
		physixx::applyAcceleration(player, elapsedTime, maths::Vec3(maths::sind(-player.rotation.y) * movementSpeed, 0.f, maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void jump(player::Player& player, float height, world::World& world)
	{
		if(world.getBlockID(player.position - maths::Vec3(0.f, player.hitbox.y, 0.f)) != blox::air)
		player.movement.y = height;
	}

	void applyGravity(player::Player& player, float elapsedTime, float gravity)
	{
		physixx::applyAcceleration(player, elapsedTime, maths::Vec3(0.f, -1.f, 0.f) * gravity);
	}
}