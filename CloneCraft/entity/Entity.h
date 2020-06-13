#pragma once

#include <cmath>
#include "../world/world.h"

struct Entity
{
	Entity();

	maths::Vec3<float> position, rotation, movement;
	maths::Vec3<float> hitbox;

	bool isStandingOnASurface(world::World& world);

	void applyMovement(float elapsedTime);

	void applyAcceleration(float elapsedTime, maths::Vec3<float> acceleration);

	void applyFriction(float elapsedTime, float friction);

	bool isColliding(world::World& world);

	void clipMovement(float elapsedTime, world::World& world);
};