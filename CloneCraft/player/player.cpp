#include "player.h"

float player::Player::playerReach = 5.f;

player::Player::Player()
{
	this->hitbox = maths::Vec3<float>(0.7f, 1.8f, 0.7f);
}

void player::Player::rotate(float X, float Y, float mouseSpeed)
{
	this->rotation.x += X * mouseSpeed;
	this->rotation.y += Y * mouseSpeed;
	if (this->rotation.x < -ROTATION_X_BOUNDS) this->rotation.x = -ROTATION_X_BOUNDS;
	if (this->rotation.x > ROTATION_X_BOUNDS) this->rotation.x = ROTATION_X_BOUNDS;

	maths::capDegrees(this->rotation.y);
}