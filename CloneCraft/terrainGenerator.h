#pragma once

#include <vector>
#include "randomNumbers.h"
#include "maths.h"

namespace terrainGen
{
	float getHeight(maths::Vec2<float> position)
	{
		if (int(position.x) % 16 == 15) position.x++;
		if (int(position.y) % 16 == 15) position.y++;
		return randomFloat::randomNumber(position.x * position.y - position.x + position.y);
	}

	void fillWithRandomNumbers(maths::Vec2<float> position, std::vector<float>& heightMap)
	{
		for (int i = 0; i < heightMap.size(); i++)
		{
			int x = i >> 4;
			int y = i - x * 16;

			heightMap[i] = getHeight(maths::Vec2<float>(float(x) + position.x, float(y) + position.y));
		}
	}

	void applyPerlinNoiseOnRow(std::vector<float>& heightMap, int rowIndex, int rowElemSize)
	{
		auto oldHeightMap = heightMap;
		for (int i = rowIndex; i < rowIndex + rowElemSize * 16; i += rowElemSize) heightMap[i] = 2.f;

		for(int exp = 1; exp < 4; exp++)
			for (int i = 0; i < 16; i += 1 << exp)
			{
				int start = rowIndex + i * rowElemSize;
				int end = start + (1 << exp) * rowElemSize;
				for (int j = start; j < end; j += rowElemSize)
				{
					heightMap[j] += (float(j - start) / float(end - start) * oldHeightMap[start] + (1.f - float(j - start) / float(end - start)) * oldHeightMap[end - 1]) / float(1 << exp);
				}
			}
	}

	void applyPerlinNoise(std::vector<float>& heightMap)
	{
		for (int i = 0; i < 256; i += 16)
			applyPerlinNoiseOnRow(heightMap, i, 1);

		for (int i = 0; i < 16; i++)
			applyPerlinNoiseOnRow(heightMap, i, 16);
	}

	std::vector<float> createHeightMap(maths::Vec2<float> position)
	{
		auto heightMap = std::vector<float>(256);

		fillWithRandomNumbers(position, heightMap);

		applyPerlinNoise(heightMap);

		return heightMap;
	}
}