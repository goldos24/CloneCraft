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
	if (position.x <= 0) position.x -= chunks::size;
	if (position.y <= 0) position.y -= chunks::size;
	if (position.z <= 0) position.z -= chunks::size;
	position.x /= chunks::size; position.y /= chunks::size; position.z /= chunks::size;
	position.x *= chunks::size; position.y *= chunks::size; position.z *= chunks::size;
	return position;
}

maths::Vec3<int> chunks::convertToChunkPos(maths::Vec3<float> position)
{
	if (position.x > 0.f && position.x < 1.f) position.x = 1.f;
	if (position.y > 0.f && position.y < 1.f) position.y = 1.f;
	if (position.z > 0.f && position.z < 1.f) position.z = 1.f;
	return convertToChunkPos(maths::convertVec3<float, int>(position));
}