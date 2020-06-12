#include "maths.h"

float maths::pythagoras(float x, float y)
{
	return sqrt(x * x + y * y);
}

void maths::capDegrees(float& angle)
{
	while (angle <= 0)
	{
		angle += 360;
	}

	while (angle >= 360)
	{
		angle -= 360;
	}
}

float maths::cosd(float angle)
{
	return (float)cos(degreesToRadians(angle));
}

float maths::sind(float angle)
{
	return (float)sin(degreesToRadians(angle));
}

float maths::tand(float angle)
{
	return (float)tan(degreesToRadians(angle));
}

float maths::atan2d(float y, float x)
{
	return radiansToDegrees(atan2f(y, x));
}

maths::Vec3<float> maths::rotateOnXf(Vec3<float> initialVector, float xRotation)
{
	return Vec3<float>(
		initialVector.x,
		initialVector.y * cosd(xRotation) - initialVector.z * sind(xRotation),
		initialVector.y * sind(xRotation) + initialVector.z * cosd(xRotation)
		);
}

bool maths::isAngleInRange(float angle, float min, float max)
{
	capDegrees(angle); capDegrees(min); capDegrees(max);
	if (max < min)
	{
		return angle <= max || angle >= min;
	}
	return angle >= min && angle <= max;
}

bool maths::is3dCoordInRange(int x, int y, int z, int min, int max)
{
	return Vec3<int>(x, y, z).isInBounds(Vec3<int>(min, min, min), Vec3<int>(max, max, max));
}

float maths::mapFromRangeToRange(float v, float aMin, float aMax, float bMin, float bMax)
{
	return bMin + (bMax - bMin) * ((v - aMin) / (aMax - aMin));
}

int maths::coord::coordinateToIndex(int x, int y, int z, int size)
{
	return x * size * size + y * size + z;
}

void maths::coord::indexToCoordinate(int i, int& x, int& y, int& z, int size)
{
	x = i / size / size;
	i -= x * size * size;
	y = i / size;
	i -= y * size;
	z = i;
	i -= z;
}

// Yeah, operator overloading isn't perfect...

std::ostream& operator<<(std::ostream& output, maths::Vec3<int> vector)
{
	return output << vector.toString();
}

std::ostream& operator<<(std::ostream& output, maths::Vec3<float> vector)
{
	return output << vector.toString();
}