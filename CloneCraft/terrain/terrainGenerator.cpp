#include "terrainGenerator.h"

float terrainGen::getHeight(maths::Vec2<float> position, float scalar)
{
	if (int(position.x) % 16 == 15) position.x += 1.f;
	if (int(position.y) % 16 == 15) position.y += 1.f;
	return randomFloat::randomNumber(position.x * position.y / scalar - position.x / scalar + position.y / scalar);
}


std::vector<float> terrainGen::createHeightMap(maths::Vec2<float> position)
{
	auto heightMap = std::vector<float>(256);

	float heights[3][3];
	for (int i = 0; i < 3; i++)
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

void terrainGen::getTreePositions(maths::Vec2<float> chunkPosition, std::vector<maths::Vec3<float>>& resultVector)
{
	auto heightMap = createHeightMap(chunkPosition);

	maths::Vec2<int> forestDataVector;
	forestDataVector.x = static_cast<int>( static_cast<int>(chunkPosition.x) >> 1 );
	forestDataVector.y = static_cast<int>(static_cast<int>(chunkPosition.y) >> 1);

	int treeCount = static_cast<int>(getHeight(maths::convertVec2<int, float>(forestDataVector), 1.f));
	if (treeCount < 0) treeCount *= -1;

	std::vector<maths::Vec3<float>> result;

	for (int i = 0; i < treeCount; ++i)
	{
		//	\\
		Calculating the local 2d Coordinate in the chunk and adjusting it if necessary
		maths::Vec3<int> localTreePosition;

		localTreePosition.x = randomFloat::randomNumber(static_cast<float>(i + forestDataVector.x * forestDataVector.y - forestDataVector.y + forestDataVector.x)) + 8.f;
		localTreePosition.z = randomFloat::randomNumber(static_cast<float>(i + forestDataVector.x * forestDataVector.y + forestDataVector.y - forestDataVector.x)) + 8.f;

		if (localTreePosition.x > 15) localTreePosition.x = 15;if (localTreePosition.z > 15) localTreePosition.z = 15;
		if (localTreePosition.x < 0) localTreePosition.x = 0;  if (localTreePosition.z < 0) localTreePosition.z = 0;

		//	\\
		Adding the height

		localTreePosition.y = heightMap[localTreePosition.z * chunks::size + localTreePosition.x] + 10.f;

		//	\\
		Pushing back the data

		result.push_back(
			maths::convertVec3<int, float>(localTreePosition) + maths::Vec3<float>(chunkPosition.x, 0.f, chunkPosition.y)
		);
	}


	std::swap<maths::Vec3<float>>(result, resultVector);
}