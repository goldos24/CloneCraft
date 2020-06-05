#pragma once

#include <cstdint>
#include "maths.h"

namespace chunks
{
	const int size = 16;

	union chunkKey
	{
		chunkKey()
		{
			this->num = 0;
		}

		uint64_t num;
		maths::genericVec3<uint16_t> vector;
	};

	chunkKey createKeyFromPosition(maths::Vec3i chunkPos)
	{
		chunkKey result;
		result.vector = maths::convertVec3<int, uint16_t>(chunkPos);
		return result;
	}


	maths::Vec3i convertToChunkPos(maths::Vec3i position)
	{
		if (position.x <= 0) position.x -= chunks::size;
		if (position.y <= 0) position.y -= chunks::size;
		if (position.z <= 0) position.z -= chunks::size;
		position.x /= chunks::size; position.y /= chunks::size; position.z /= chunks::size;
		position.x *= chunks::size; position.y *= chunks::size; position.z *= chunks::size;
		return position;
	}

	maths::Vec3i convertToChunkPos(maths::Vec3 position)
	{
		return convertToChunkPos(maths::convertVec3<float, int>(position));
	}
}