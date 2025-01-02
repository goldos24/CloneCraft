#include "terrainGenerator.h"
#include "perlin.h"
#include "../game/voxelConstants.h"

float terrainGen::getHeight(maths::Vec2<float> position, float scalar)
{
	if (int(position.x) % 16 == 15) position.x += 1.f;
	if (int(position.y) % 16 == 15) position.y += 1.f;
	return randomFloat::randomNumber(position.x * position.y / scalar - position.x / scalar + position.y / scalar);
}


std::vector<float> terrainGen::createHeightMap(maths::Vec2<float> position, float seed)
{
	auto heightMap = std::vector<float>(256);

	maths::Vec2<float> noiseOriginPosition = { floor(position.x / getVoxelSubdivision() / 16) * 16, floor(position.y / getVoxelSubdivision() / 16) * 16 };
	maths::Vec2<float>data[4];
	perlin::generateData(chunks::size, seed, noiseOriginPosition, data);
	maths::Vec2<float> chunkOriginPosition = maths::Vec2<float>(floor(position.x / 16) * 16, floor(position.y / 16) * 16) * (float)getFaceSize();
	for (int i = 0; i < heightMap.size(); i++)
	{
		float x = i >> 4;
		float y = i - x * 16;
		heightMap[i] = perlin::getPoint(chunks::size, seed, chunkOriginPosition + maths::Vec2<float>{(0.5f + x) / getVoxelSubdivision(), (0.5f + y) / getVoxelSubdivision()}, data) - 8;
	}

	return heightMap;
}

void terrainGen::getTreePositions(maths::Vec2<float> chunkPosition, std::vector<maths::Vec3<float>>& resultVector, float seed)
{
	auto heightMap = createHeightMap(chunkPosition, seed);

	maths::Vec2<int> forestDataVector;
	forestDataVector.x = static_cast<int>( static_cast<int>(chunkPosition.x + seed) >> 3 );
	forestDataVector.y = static_cast<int>(static_cast<int>(chunkPosition.y + seed) >> 2);

	int treeCount = static_cast<int>(getHeight(maths::convertVec2<int, float>(forestDataVector), 1.f));
	if (treeCount < 0) treeCount = 0;


	for (int i = 0; i < treeCount; ++i)
	{
		//	\\
		Calculating the local 2d Coordinate in the chunk and adjusting it if necessary
		maths::Vec3<int> localTreePosition;

		localTreePosition.x = randomFloat::randomNumber(static_cast<float>(i + forestDataVector.x * forestDataVector.y - forestDataVector.y + forestDataVector.x)) + 8.f;
		localTreePosition.z = randomFloat::randomNumber(static_cast<float>(i + forestDataVector.x * forestDataVector.y + forestDataVector.y - forestDataVector.x)) + 8.f;

		if (localTreePosition.x < 0) localTreePosition.x *= -1;  if (localTreePosition.z < 0) localTreePosition.z *= -1;
		if (localTreePosition.x > 15) localTreePosition.x = 15 - localTreePosition.x;if (localTreePosition.z > 15) localTreePosition.z = 15 - localTreePosition.z;

		//	\\
		Adding the height
		
		localTreePosition.y = 12.f + heightMap[localTreePosition.x * chunks::size + localTreePosition.z];

		//	\\
		Pushing back the data

		resultVector.push_back(
			maths::convertVec3<int, float>(localTreePosition) + maths::Vec3<float>(chunkPosition.x, 0.f, chunkPosition.y)
		);
	}
}
