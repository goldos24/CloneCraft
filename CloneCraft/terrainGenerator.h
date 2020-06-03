#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include "randomNumbers.h"
#include "maths.h"

namespace terrainGen
{
	float getHeight(maths::Vec2<float> position, float scalar)
	{
		if (int(position.x) % 16 == 15) position.x += 1.f;
		if (int(position.y) % 16 == 15) position.y += 1.f;
		return randomFloat::randomNumber(position.x * position.y / scalar - position.x / scalar + position.y / scalar);
	}

	void fillWithRandomNumbers(maths::Vec2<float> position, std::vector<float>& heightMap)
	{
		for (int i = 0; i < heightMap.size(); i++)
		{
			int x = i >> 4;
			int y = i - x * 16;

			heightMap[i] = getHeight(maths::Vec2<float>(float(x) + position.x, float(y) + position.y), 1.f);
		}
	}

	void applyPerlinNoiseOnRow(std::vector<float>& heightMap, int rowIndex, int rowElemSize)
	{
		std::vector<float> newHeightMap = std::vector<float>(heightMap.size());
		
		float average = 0.f;

		for (int i = rowIndex; i < rowIndex + rowElemSize * 16; i += rowElemSize) average += heightMap[i] / 16.f;

		int IterationCount = 2;
		for (int j = 0; j < IterationCount; j++)
		{
			for (int i = rowIndex; i < rowIndex + rowElemSize * 16; i += rowElemSize) newHeightMap[i] += (heightMap[i]
				+ i > rowIndex + rowElemSize ? heightMap[i - rowElemSize] : heightMap[i]
				+ i < rowIndex + rowElemSize * 15 ? heightMap[i + rowElemSize] : heightMap[i]) / float(IterationCount);

			for (int i = rowIndex; i < rowIndex + rowElemSize * 16; i += rowElemSize) heightMap[i] = newHeightMap[i];
		}
	}

	void applyPerlinNoise(std::vector<float>& heightMap)
	{
		for (int i = 0; i < 256; i += 16)
			applyPerlinNoiseOnRow(heightMap, i, 1);

		for (int i = 0; i < 16; i++) 
			applyPerlinNoiseOnRow(heightMap, i, 16);

		for (int i = 0; i < 256; i += 16)
			applyPerlinNoiseOnRow(heightMap, i, 1);
	}

	std::vector<float> createHeightMap(maths::Vec2<float> position)
	{
		auto heightMap = std::vector<float>(256);

		fillWithRandomNumbers(position, heightMap);

		applyPerlinNoise(heightMap);

		/*for (int i = 0; i < 16; i++)
		{
			std::cout << "[";
			for (int j = 0; j < 16; j++)
				std::cout << heightMap[i * 16 + j] << (j < 15 ? "," : "]\n\r");
		}*/

		return heightMap;
	}
}