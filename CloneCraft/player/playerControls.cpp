#include "playerControls.h"


void playerControls::moveUp(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.position.y += movementSpeed * elapsedTime;
}

void playerControls::moveDown(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.position.y -= movementSpeed * elapsedTime;
}

void playerControls::moveRight(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.applyAcceleration(elapsedTime, maths::Vec3<float>(maths::cosd(-player.rotation.y) * movementSpeed, 0.f, -maths::sind(-player.rotation.y) * movementSpeed));
}

void playerControls::moveLeft(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.applyAcceleration(elapsedTime, maths::Vec3<float>(-maths::cosd(-player.rotation.y) * movementSpeed, 0.f, maths::sind(-player.rotation.y) * movementSpeed));
}

void playerControls::moveForward(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.applyAcceleration(elapsedTime, maths::Vec3<float>(-maths::sind(-player.rotation.y) * movementSpeed, 0.f, -maths::cosd(-player.rotation.y) * movementSpeed));
}

void playerControls::moveBackward(player::Player& player, float elapsedTime, float movementSpeed)
{
	player.applyAcceleration(elapsedTime, maths::Vec3<float>(maths::sind(-player.rotation.y) * movementSpeed, 0.f, maths::cosd(-player.rotation.y) * movementSpeed));
}

void playerControls::jump(player::Player& player, float height, world::World& world)
{
	if (player.isStandingOnASurface(world))
		player.movement.y = height;
}

void playerControls::applyGravity(player::Player& player, float elapsedTime, float gravity)
{
	player.applyAcceleration(elapsedTime, maths::Vec3<float>(0.f, -1.f, 0.f) * gravity);
}