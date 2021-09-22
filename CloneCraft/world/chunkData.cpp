#include "chunkData.h"

chunks::chunkKey::chunkKey()
{
	this->num = 0;
}

chunks::chunkKey chunks::createKeyFromPosition(maths::Vec3<int> chunkPos)
{
	chunkKey result;
	result.vector = maths::convertVec3<int, uint16_t>(chunkPos);
	return result;
}


maths::Vec3<int> chunks::convertToChunkPos(maths::Vec3<int> position)
{
	maths::Vec3<unsigned int> uposition = maths::convertVec3<int, unsigned int>(position);
	uposition.x /= chunks::size; uposition.y /= chunks::size; uposition.z /= chunks::size;
	uposition.x *= chunks::size; uposition.y *= chunks::size; uposition.z *= chunks::size;
	return maths::convertVec3<unsigned int, int>(uposition);
}

maths::Vec3<int> chunks::convertToChunkPos(maths::Vec3<float> position)
{
	//if (position.x > 0.f && position.x < 1.f) position.x = 1.f;
	//if (position.y > 0.f && position.y < 1.f) position.y = 1.f;
	//if (position.z > 0.f && position.z < 1.f) position.z = 1.f;
	position.x = floor(position.x);
	position.y = floor(position.y);
	position.z = floor(position.z);
	return convertToChunkPos(maths::convertVec3<float, int>(position));
}
