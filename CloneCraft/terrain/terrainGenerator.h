#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include "../world/chunkData.h"
#include "randomNumbers.h"
#include "../maths/maths.h"

namespace terrainGen
{
	float getHeight(maths::Vec2<float> position, float scalar);
	std::vector<float> createHeightMap(maths::Vec2<float> position);
	void getTreePositions(maths::Vec2<float> chunkPosition, std::vector<maths::Vec3<float>>& resultVector);
}