#pragma once

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
	};
}