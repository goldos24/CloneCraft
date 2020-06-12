#pragma once

#include <cstdint>
#include "../maths/maths.h"

namespace chunks
{
	const int size = 16;

	union chunkKey
	{
		chunkKey();

		uint64_t num;
		maths::Vec3<uint16_t> vector;
	};

	chunkKey createKeyFromPosition(maths::Vec3<int> chunkPos);


	maths::Vec3<int> convertToChunkPos(maths::Vec3<int> position);

	maths::Vec3<int> convertToChunkPos(maths::Vec3<float> position);
}