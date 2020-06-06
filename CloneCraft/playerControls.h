#pragma once
#include "player.h"
#include "physics.h"

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
}