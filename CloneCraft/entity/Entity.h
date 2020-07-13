#pragma once

#include <cmath>
#include <functional>
#include "../world/forwardDeclarations.h"
#include "../maths/maths.h"
#include "../game/forwardDeclarations.h"

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

	virtual void renderModel();

	void render(maths::Vec3<float> cameraPosition, maths::Vec3<float> cameraRotation);

	virtual void update(Game&, float);
};