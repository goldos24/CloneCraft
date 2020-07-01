#pragma once

#include "../entity/Entity.h"

struct Sheep : Entity
{
	Sheep();

	void renderModel();

	void drawLeg(maths::Vec3<float> position);
};