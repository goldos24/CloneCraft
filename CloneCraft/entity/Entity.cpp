#include "Entity.h"

Entity::Entity()
{}

bool Entity::isStandingOnASurface(world::World& world)
{
	for (float i = -this->hitbox.x / 2.f; i <= this->hitbox.x / 2.f; i += this->hitbox.x / 2.f)
		for (float k = -this->hitbox.z / 2.f; k <= this->hitbox.z / 2.f; k += this->hitbox.z / 2.f)
		{
			if (world.getBlockID(this->position + maths::Vec3<float>(i, -this->hitbox.y - 1.f, k)) != blox::air &&
				world.getBlockID(this->position + maths::Vec3<float>(i, -this->hitbox.y, k)) == blox::air &&
				this->position.y - this->hitbox.y >= floorf(this->position.y - this->hitbox.y) - 0.01f && this->position.y - this->hitbox.y < floorf(this->position.y - this->hitbox.y) + 0.01f
				) return true;
		}
	return false;
}


void Entity::applyMovement(float elapsedTime)
{
	this->position += this->movement * elapsedTime;
}

void Entity::applyAcceleration(float elapsedTime, maths::Vec3<float> acceleration)
{
	this->movement += acceleration * elapsedTime;
}

void Entity::applyFriction(float elapsedTime, float friction)
{
	float frictionFactor = (1 < friction * elapsedTime) ? 0.f : 1 - friction * elapsedTime;
	this->movement = this->movement * frictionFactor;
}

bool Entity::isColliding(world::World& world)
{
	for (float i = -this->hitbox.x / 2.f; i <= this->hitbox.x / 2.f; i += this->hitbox.x / 2.f)
		for (float j = 0; j < this->hitbox.y; j++)
			for (float k = -this->hitbox.z / 2.f; k <= this->hitbox.z / 2.f; k += this->hitbox.z / 2.f)
			{
				if (world.getBlockID(this->position + maths::Vec3<float>(
					i > this->hitbox.x / 2.f ? this->hitbox.x / 2.f : i,
					-(j + 1 > this->hitbox.y ? this->hitbox.y : j),
					k > this->hitbox.z / 2.f ? this->hitbox.z / 2.f : k
					)) != blox::air)
					return true;
			}
	return false;
}

void Entity::clipMovement(float elapsedTime, world::World& world)
{
	auto appliedMovementVector = this->movement * elapsedTime;

	this->position += maths::Vec3<float>(appliedMovementVector.x, 0.f, 0.f);
	if (this->isColliding(world)) this->movement.x = 0.f;
	this->position -= maths::Vec3<float>(appliedMovementVector.x, 0.f, 0.f);

	this->position += maths::Vec3<float>(0.f, appliedMovementVector.y, 0.f);
	if (this->isColliding(world)) { this->movement.y = 0.f; }
	this->position -= maths::Vec3<float>(0.f, appliedMovementVector.y, 0.f);

	this->position += maths::Vec3<float>(0.f, 0.f, appliedMovementVector.z);
	if (this->isColliding(world)) this->movement.z = 0.f;
	this->position -= maths::Vec3<float>(0.f, 0.f, appliedMovementVector.z);
}