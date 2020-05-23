#pragma once
#include "blocks.h"

namespace chunks 
{

	const int size = 16;

	auto coordinateToIndex(int x, int y, int z) -> int
	{
		return x * size * size + y * size + z;
	}

	auto indexToCoordinate(int i, int& x, int& y, int& z)
	{
		x = i / size / size;
		i -= x * size * size;
		y = i / size;
		i -= y * size;
		z = i;
		i -= z;
	}

	struct Chunk
	{
		blox::ID blocks[size * size * size];
		auto getBlock(int x, int y, int z)
		{
			return this-> blocks[ coordinateToIndex(x, y, z) ];
		}
	};
}