#pragma once
#include <math.h>
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
	struct Vec3
	{
		Vec3() {}

		Vec3(num x, num y, num z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		num x = 0.f;
		num y = 0.f;
		num z = 0.f;

		Vec3<num> operator * (num factor)
		{
			return Vec3<num>(
				this->x * factor,
				this->y * factor,
				this->z * factor
			);
		}

		Vec3<num> operator *= (num factor)
		{
			*this = *this * factor;
			return *this;
		}

		Vec3<num> operator / (Vec3<num> theOtherVec3) // wtf?
		{
			return Vec3<num>(
				this->x / theOtherVec3.x,
				this->y / theOtherVec3.y,
				this->z / theOtherVec3.z
				);
		}

		Vec3<num> operator / (num divisor)
		{
			return *this * (num(1) / divisor);
		}

		Vec3<num> operator + (Vec3<num> theOtherVec3)
		{
			return Vec3<num>(
				this->x + theOtherVec3.x,
				this->y + theOtherVec3.y,
				this->z + theOtherVec3.z
				);
		}

		Vec3<num> operator - (Vec3<num> theOtherVec3)
		{
			return Vec3<num>(
				this->x - theOtherVec3.x,
				this->y - theOtherVec3.y,
				this->z - theOtherVec3.z
				);
		}

		Vec3<num> floor(num unitSize)
		{
			auto resultDouble = Vec3<double>(round(double(this->x) / double(unitSize) - 0.5), round(double(this->y) / double(unitSize) - 0.5), round(double(this->z) / double(unitSize) - 0.5));
			resultDouble *= double(unitSize);
			return Vec3<num>( num(resultDouble.x), num(resultDouble.y), num(resultDouble.z));
		}

		Vec3<num> ceil(num unitSize)
		{
			auto resultDouble = Vec3<double>(round(double(this->x) / double(unitSize) + 0.499), round(double(this->y) / double(unitSize) + 0.499), round(double(this->z) / double(unitSize) + 0.499));
			resultDouble *= double(unitSize);
			return Vec3<num>(num(resultDouble.x), num(resultDouble.y), num(resultDouble.z));
		}

		Vec3<num>& operator += (Vec3<num> theOtherVec3)
		{
			this->x += theOtherVec3.x;
			this->y += theOtherVec3.y;
			this->z += theOtherVec3.z;

			return *this;
		}

		Vec3<num>& operator -= (Vec3<num> theOtherVec3)
		{
			this->x -= theOtherVec3.x;
			this->y -= theOtherVec3.y;
			this->z -= theOtherVec3.z;

			return *this;
		}

		bool isInBounds(Vec3<num> a, Vec3<num> b)
		{
			return
				this->x >= a.x &&
				this->y >= a.y &&
				this->z >= a.z &&
				this->x < b.x &&
				this->y < b.y &&
				this->z < b.z;
		}

		num overlySimpleDistanceTo(Vec3<num> otherVec3)
		{
			num distance = abs(this->x - otherVec3.x);
			if(abs(this->y - otherVec3.y) > distance) distance = abs(this->y - otherVec3.y);
			if (abs(this->z - otherVec3.z) > distance) distance = abs(this->z - otherVec3.z);
			return distance;
		}

		std::string toString()
		{
			return "< x : " + std::to_string(this->x) + ", y : " + std::to_string(this->y) + ", z : " + std::to_string(this->z) + " >";
		}

		std::string toParseableString()
		{
			return std::to_string(this->x) + " " + std::to_string(this->y) + " " + std::to_string(this->z);
		}

		void loadFromIstream(std::istream& stream)
		{
			stream >> this->x >> this->y >> this->z;
		}

		bool operator == (Vec3<num>& theOtherVec3)
		{
			return this->x == theOtherVec3.x &&
				this->y == theOtherVec3.y &&
				this->z == theOtherVec3.z;
		}

		bool operator == (Vec3<num>&& theOtherVec3)
		{
			return this->x == theOtherVec3.x &&
				this->y == theOtherVec3.y &&
				this->z == theOtherVec3.z;
		}

		bool operator == (Vec3<num>* theOtherVec3)
		{
			return this->x == theOtherVec3->x &&
				this->y == theOtherVec3->y &&
				this->z == theOtherVec3->z;
		}
	};

	template <class sourceNum, class targetNum> 
	Vec3<targetNum> convertVec3(Vec3<sourceNum> source)
	{
		return Vec3<targetNum>(targetNum(source.x), targetNum(source.y), targetNum(source.z));
	}

	namespace unitVectors
	{
		// Shorthand for the vector (0, 1, 0)
		static const Vec3<int> up(0, 1, 0);
		// Shorthand for the vector (0, -1, 0)
		static const Vec3<int> down(0, -1, 0);
		// Shorthand for the vector (1, 0, 0)
		static const Vec3<int> left(1, 0, 0);
		// Shorthand for the vector (-1, 0, 0)
		static const Vec3<int> right(-1, 0, 0);
		// Shorthand for the vector (0, 0, 1)
		static const Vec3<int> front(0, 0, 1);
		// Shorthand for the vector (0, 0, -1)
		static const Vec3<int> back(0, 0, -1);
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
			return Vec2<num>(
				this->x * factor,
				this->y * factor
				);
		}

		Vec2<num> operator / (num scalarnt)
		{
			return Vec2<num>(
				this->x / scalarnt,
				this->y / scalarnt
				);
		}

		Vec2<num> operator + (Vec2<num> theOtherVec3)
		{
			return Vec2<num>(
				this->x + theOtherVec3.x,
				this->y + theOtherVec3.y
				);
		}

		Vec2<num> operator - (Vec2<num> theOtherVec3)
		{
			return Vec2<num>(
				this->x - theOtherVec3.x,
				this->y - theOtherVec3.y
				);
		}

		num operator * (Vec2<num> theOtherVec3)
		{
			return this->x * theOtherVec3.x + this->y * theOtherVec3.y;
		}

		bool isInBounds(Vec2<num> a, Vec2<num> b)
		{
			return
				this->x >= a.x &&
				this->y >= a.y &&
				this->x < b.x  &&
				this->y < b.y
				;
		}
	};

	template <class sourceNum, class targetNum>
	Vec2<targetNum> convertVec2(Vec2<sourceNum> sauce)
	{
		return Vec2<targetNum>(targetNum(sauce.x), targetNum(sauce.y));
	}

	template<class num>
	num dotProduct(int dimensions, num* vector1, num* vector2)
	{
		num result(0);
		for (int i = 0; i < dimensions; ++i)
			result += vector1[i] * vector2[i];

		return result;
	}

	float pythagoras(float x, float y);

	constexpr float pi = 3.1415926535897932384626433832795;

	constexpr float degreesToRadians(float degrees)
	{
		return degrees / 180 * pi;
	}

	constexpr float radiansToDegrees(float radians)
	{
		return radians * 180 / pi;
	}

	void capDegrees(float& angle);
	
	template <typename T>
	T abs(T value)
	{
		return value < 0 ? -value : value;
	}

	float cosd(float angle);

	float sind(float angle);

	float tand(float angle);

	float atan2d(float y, float x);

	Vec3<float> rotateOnXf(Vec3<float> initialVector, float xRotation);

	bool isAngleInRange(float angle, float min, float max);

	template <class num>
	Vec3<num> positionFromRotation(Vec3<num> rotation)
	{
		num fixedRotationX = rotation.x + num(90);
		num rightAngleMinusFixedRotationX = num(90) - fixedRotationX;
		num rightAngleMinusRotationY = num(90) - rotation.y;

		num tX = cosd(rightAngleMinusRotationY) * abs(cosd(rightAngleMinusFixedRotationX));
		num tY = sind(rightAngleMinusFixedRotationX);
		num tZ = sind(-rightAngleMinusRotationY) * abs(cosd(rightAngleMinusFixedRotationX));
		return Vec3<num>(tX, tY, tZ);
	}

	constexpr float cubeof(float x) { return x * x * x; };
	constexpr int cubeof(int x) { return x * x * x; };

	constexpr float squareof(float x) { return x * x; };
	constexpr int squareof(int x) { return x * x; };

	bool is3dCoordInRange(int x, int y, int z, int min, int max);

	float mapFromRangeToRange(float v, float aMin, float aMax, float bMin, float bMax);

	namespace coord
	{
		constexpr int coordinateToIndex(int x, int y, int z, int size)
		{
			return x * size * size + y * size + z;
		}

		void indexToCoordinate(int i, int& x, int& y, int& z, int size);
	}
}

std::ostream& operator<<(std::ostream& output, maths::Vec3<int> vector);
std::ostream& operator<<(std::ostream& output, maths::Vec3<float> vector);
