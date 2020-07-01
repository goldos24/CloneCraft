#include "player.h"
#include "../game/game.h"

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

void player::Player::renderModel()
{

}

void player::Player::saveDataToFile(std::string worldFileName)
{
	std::ofstream out(worldFileName + ".pdata");
	if (!out) return;

	float x = this->position.x, y = this->position.y, z = this->position.z;
	float rX = this->rotation.x, rY = this->rotation.y, rZ = this->rotation.z;

	out << x << std::endl;
	out << y << std::endl;
	out << z << std::endl;

	out << rX << std::endl;
	out << rY << std::endl;
	out << rZ << std::endl;

	out.close();
}

void player::Player::loadDataFromFile(std::string worldFileName)
{
	std::ifstream in(worldFileName + ".pdata");
	if (!in) return;

	float x, y, z;
	float rX, rY, rZ;

	in >>  x >>  y >>  z;
	in >> rX >> rY >> rZ;

	in.close();

	this->position = maths::Vec3<float>(x, y, z);
	this->rotation = maths::Vec3<float>(rX, rY, rZ);
}

void player::Player::update(Game& game, float elapsedTime)
{
	game.updatePosition(elapsedTime);
}