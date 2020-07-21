#pragma once

#include "../maths/maths.h"
#include "randomNumbers.h"

namespace perlin
{
	float getPoint(float gridSize, float seed, maths::Vec2<float> pos, maths::Vec2<float>* data);

	void generateData(float gridSize, float seed, maths::Vec2<float> pos, maths::Vec2<float>* data);

	maths::Vec2<float> getVectorAtPosition(float seed, maths::Vec2<float> pos);
}