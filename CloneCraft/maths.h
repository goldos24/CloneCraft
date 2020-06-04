#pragma once
#include <math.h>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <iostream>

namespace maths
{
	template<class num> num compareThreeWay(num a, num b)
	{
		num result = num(0);
		if (a < b) result--;
		if (a > b) result++;
		return result;
	}

	typedef unsigned char uint8;
	typedef unsigned short uint16;

	template<class num>
	struct genericVec3
	{
		genericVec3() {}

		genericVec3(num x, num y, num z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		num x = 0.f;
		num y = 0.f;
		num z = 0.f;

		genericVec3<num> operator * (num factor)
		{
			return genericVec3<num>(
				this->x * factor,
				this->y * factor,
				this->z * factor
			);
		}

		genericVec3<num> operator *= (num factor)
		{
			*this = *this * factor;
			return *this;
		}

		genericVec3<num> operator / (genericVec3<num> theOtherVec3) // wtf?
		{
			return genericVec3<num>(
				this->x / theOtherVec3.x,
				this->y / theOtherVec3.y,
				this->z / theOtherVec3.z
				);
		}

		genericVec3<num> operator / (num divisor)
		{
			return *this * (num(1) / divisor);
		}

		genericVec3<num> operator + (genericVec3<num> theOtherVec3)
		{
			return genericVec3<num>(
				this->x + theOtherVec3.x,
				this->y + theOtherVec3.y,
				this->z + theOtherVec3.z
				);
		}

		genericVec3<num> operator - (genericVec3<num> theOtherVec3)
		{
			return genericVec3<num>(
				this->x - theOtherVec3.x,
				this->y - theOtherVec3.y,
				this->z - theOtherVec3.z
				);
		}

		genericVec3<num> floor(num unitSize)
		{
			auto resultDouble = genericVec3<double>(round(double(this->x) / double(unitSize) - 0.5), round(double(this->y) / double(unitSize) - 0.5), round(double(this->z) / double(unitSize) - 0.5));
			resultDouble *= double(unitSize);
			return genericVec3<num>( num(resultDouble.x), num(resultDouble.y), num(resultDouble.z));
		}

		auto& operator += (genericVec3<num>&& theOtherVec3)
		{
			this->x += theOtherVec3.x;
			this->y += theOtherVec3.y;
			this->z += theOtherVec3.z;

			return *this;
		}

		bool isInBounds(genericVec3<num> a, genericVec3<num> b)
		{
			return
				this->x >= a.x &&
				this->y >= a.y &&
				this->z >= a.z &&
				this->x < b.x &&
				this->y < b.y &&
				this->z < b.z;
		}

		num overlySimpleDistanceTo(genericVec3<num> otherVec3)
		{
			num distance = abs(this->x - otherVec3.x);
			if(abs(this->y - otherVec3.y) > distance) distance = abs(this->y - otherVec3.y);
			if (abs(this->z - otherVec3.z) > distance) distance = abs(this->z - otherVec3.z);
			return distance;
		}

		std::string toString()
		{
			return "( x : " + std::to_string(this->x) + ", y : " + std::to_string(this->y) + ", z : " + std::to_string(this->z) + " )";
		}

		bool operator == (genericVec3<num>& theOtherVec3)
		{
			return this->x == theOtherVec3.x &&
				this->y == theOtherVec3.y &&
				this->z == theOtherVec3.z;
		}

		bool operator == (genericVec3<num>&& theOtherVec3)
		{
			return this->x == theOtherVec3.x &&
				this->y == theOtherVec3.y &&
				this->z == theOtherVec3.z;
		}

		bool operator == (genericVec3<num>* theOtherVec3)
		{
			return this->x == theOtherVec3->x &&
				this->y == theOtherVec3->y &&
				this->z == theOtherVec3->z;
		}
	};


	template <class sourceNum, class targetNum> 
	genericVec3<targetNum> convertVec3(genericVec3<sourceNum> source)
	{
		return genericVec3<targetNum>(targetNum(source.x), targetNum(source.y), targetNum(source.z));
	}

	typedef genericVec3<float> Vec3;
	typedef genericVec3<int> Vec3i;

	namespace unitVectors
	{
		// Shorthand for the vector (0, 1, 0)
		static const Vec3i up(0, 1, 0);
		// Shorthand for the vector (0, -1, 0)
		static const Vec3i down(0, -1, 0);
		// Shorthand for the vector (1, 0, 0)
		static const Vec3i left(1, 0, 0);
		// Shorthand for the vector (-1, 0, 0)
		static const Vec3i right(-1, 0, 0);
		// Shorthand for the vector (0, 0, 1)
		static const Vec3i front(0, 0, 1);
		// Shorthand for the vector (0, 0, -1)
		static const Vec3i back(0, 0, -1);
	}

	template<class num>
	struct Vec2
	{
		Vec2() {}

		Vec2(num x, num y)
		{
			this->x = x;
			this->y = y;
		}
		num x = 0.f;
		num y = 0.f;

		Vec2<num> operator * (num factor)
		{
			return genericVec3<num>(
				this->x * factor,
				this->y * factor
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
		while (angle <= 0)
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

	float tand(float angle)
	{
		return (float)tan(degreesToRadians(angle));
	}

	constexpr float cubeof(float x) { return x * x * x; };
	constexpr int cubeof(int x) { return x * x * x; };

	constexpr float squareof(float x) { return x * x; };
	constexpr int squareof(int x) { return x * x; };

	bool isInRange(int v, int min, int max)
	{
		return v >= min && v < max;
	}

	bool isInRangeF(float v, float min, float max)
	{
		return v >= min && v < max;
	}

	bool is3dCoordInRange(int x, int y, int z, int min, int max)
	{
		return
			isInRange(x, min, max) &&
			isInRange(y, min, max) &&
			isInRange(z, min, max);
	}

	float mapFromRangeToRange(float v, float aMin, float aMax, float bMin, float bMax)
	{
		return bMin + (bMax - bMin) * ((v - aMin) / (aMax - aMin));
	}

	Vec3i convertFromVec3ToVec3i(Vec3 vec) // TODO fucking remove 'cause we're using generics to type fewer functions
	{
		return convertVec3<float, int>(vec);
	}

	namespace coord
	{
		auto coordinateToIndex(int x, int y, int z, int size) -> int
		{
			return x * size * size + y * size + z;
		}

		auto indexToCoordinate(int i, int& x, int& y, int& z, int size)
		{
			x = i / size / size;
			i -= x * size * size;
			y = i / size;
			i -= y * size;
			z = i;
			i -= z;
		}
	}

	namespace color
	{
		uint8 normalizeChannel(uint8 channel)
		{
			return uint8( sqrt( float(channel) / 255 ) * 255.f );
		}

		void normalizeRef(sf::Color& color)
		{
			/*color = sf::Color(
				normalizeChannel(color.r),
				normalizeChannel(color.g),
				normalizeChannel(color.b),
				color.a
			);*/
		}
	}
}


// Yeah, operator overloading isn't perfect...

std::ostream& operator<<(std::ostream& output, maths::Vec3i vector)
{
	return output << vector.toString();
}

std::ostream& operator<<(std::ostream& output, maths::Vec3 vector)
{
	return output << vector.toString();
}