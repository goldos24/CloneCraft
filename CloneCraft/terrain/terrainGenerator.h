#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include "../world/chunkData.h"
#include "randomNumbers.h"
#include "../maths/maths.h"

namespace terrainGen
{
	constexpr size_t HEIGHTMAP_SIZE = 256;
	float getHeight(maths::Vec2<float> position, float scalar);
	void createHeightMap(maths::Vec2<float> position, float seed, float heightMap[HEIGHTMAP_SIZE]);
	void getTreePositions(maths::Vec2<float> chunkPosition, std::vector<maths::Vec3<float>>& resultVector, float seed);
}
