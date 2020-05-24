#pragma once
#include <math.h>

namespace maths
{
	struct Vec3
	{
		Vec3() {}

		Vec3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		Vec3 operator * (float factor)
		{
			return Vec3(
				this-> x * factor,
				this-> y * factor,
				this-> z * factor
			);
		}
	};

	const float pi = 3.1415926535897932384626433832795;

	auto degreesToRadians(float degrees)
	{
		return degrees / 180 * pi;
	}

	void capDegrees(float& angle)
	{
		while (angle < 0)
		{
			angle += 360;
		}

		while (angle >= 360)
		{
			angle -= 360;
		}
	}
	
	float cosd(float angle)
	{
		return (float)cos(degreesToRadians(angle));
	}

	float sind(float angle)
	{
		return (float)sin(degreesToRadians(angle));
	}
}