#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include "../world/chunkData.h"
#include "randomNumbers.h"
#include "../maths/maths.h"

namespace terrainGen
{
	float getHeight(maths::Vec2<float> position, float scalar)
	{
		if (int(position.x) % 16 == 15) position.x += 1.f;
		if (int(position.y) % 16 == 15) position.y += 1.f;
		return randomFloat::randomNumber(position.x * position.y / scalar - position.x / scalar + position.y / scalar);
	}


	std::vector<float> createHeightMap(maths::Vec2<float> position)
	{
		auto heightMap = std::vector<float>(256);

		float heights[3][3];
		for (int i = 0; i <3; i++)
			for (int j = 0; j < 3; j++)
			{
				heights[i][j] = getHeight((position / 16.f) + maths::Vec2<float>(float(i), float(j)), 1.f);
			}
		for (int i = 0; i < heightMap.size(); i++)
		{
			auto height = heights[1][1];
			int x = i >> 4;
			int y = i - x * 16;
			float xInterpolation = float(x - chunks::size / 2) / float(chunks::size);
			float yInterpolation = float(y - chunks::size / 2) / float(chunks::size);
			float divisor = 1.f + (abs(xInterpolation) > abs(yInterpolation) ? abs(xInterpolation) : abs(yInterpolation));
			height /= (divisor + 1.f);
			height += (xInterpolation > 0 ? heights[2][1] : heights[0][1]) * abs(xInterpolation);
			height += (yInterpolation > 0 ? heights[1][2] : heights[1][0]) * abs(yInterpolation);
			heightMap[i] = height / divisor / 1.f - 6.f;
		}

		return heightMap;
	}
}