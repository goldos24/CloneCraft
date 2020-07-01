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
		for (float j = 0; j < this->hitbox.y + 0.5f; j += 0.5f)
			for (float k = -this->hitbox.z / 2.f; k <= this->hitbox.z / 2.f; k += this->hitbox.z / 2.f)
			{
				if (world.getBlockID(this->position + maths::Vec3<float>(
					i > this->hitbox.x / 2.f ? this->hitbox.x / 2.f : i,
					-(j > this->hitbox.y ? this->hitbox.y : j),
					k > this->hitbox.z / 2.f ? this->hitbox.z / 2.f : k
					)) != blox::air)
					return true;
			}
	return false;
}

void Entity::clipMovement(float elapsedTime, world::World& world)
{
	auto appliedMovementVector = this->movement * elapsedTime;
	auto possibleNewPosition = this->position;
	auto initialEntityPosition = this->position;

	this->position.x += appliedMovementVector.x;
	if (this->isColliding(world))
	{
		this->movement.x = 0.f;
		this->position.x -= appliedMovementVector.x;
	}

	this->position.y += appliedMovementVector.y;
	if (this->isColliding(world))
	{
		this->movement.y = 0.f;
		this->position.y -= appliedMovementVector.y;
	}

	this->position.z += appliedMovementVector.z;
	if (this->isColliding(world))
	{
		this->movement.z = 0.f;
		this->position.z -= appliedMovementVector.z;
	}

	this->position = initialEntityPosition;
}

void Entity::renderModel()
{

}

void Entity::render(maths::Vec3<float> cameraPosition, maths::Vec3<float> cameraRotation)
{
	// Restoring the identity (= transformedn't) matrix manually because OpenGL sometimes sucks

	glLoadIdentity();

	// Entity Transformations

	// Camera Transformations
	glRotatef(cameraRotation.x, 1.f, 0.f, 0.f);
	glRotatef(-cameraRotation.y, 0.f, -1.f, 0.f);
	glTranslatef(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z);

	// Entity Translation

	glTranslatef(this->position.x, this->position.y - this->hitbox.y, this->position.z);

	// Finally Drawing the Entity
	this->renderModel();
}

void Entity::update(Game& game, float elapsedTime)
{
	
}