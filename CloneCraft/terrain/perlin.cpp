#include "perlin.h"


namespace perlin
{

	float getPoint(float gridSize, float seed, maths::Vec2<float> pos, maths::Vec2<float>* data)
	{
		float xStart, yStart, xEnd, yEnd;
		xStart = floor(pos.x / gridSize) * gridSize;
		yStart = floor(pos.y / gridSize) * gridSize;
		xEnd = xStart + gridSize;
		yEnd = yStart + gridSize;

		maths::Vec2<float> corners[4] = 
		{
			{xStart, yStart},
			{xStart, yEnd},
			{xEnd, yStart},
			{xEnd, yEnd}
		};
		
		float dotProducts[4];
		for (int i = 0; i < 4; ++i)
		{

			dotProducts[i] = (corners[i] - pos) * data[i];
		}

		float
			xInterpolation = maths::cosd((pos.x - xStart) / gridSize  * 90.f),
			yInterpolation = maths::cosd((pos.y - yStart) / gridSize  * 90.f);

		float
			upperDotProduct = dotProducts[1] * xInterpolation + dotProducts[3] * (1 - xInterpolation),
			lowerDotProduct = dotProducts[0] * xInterpolation + dotProducts[2] * (1 - xInterpolation);

		return yInterpolation * lowerDotProduct + (1 - yInterpolation) * upperDotProduct;
	}


	void generateData(float gridSize, float seed, maths::Vec2<float> pos, maths::Vec2<float>* data)
	{
		float xStart, yStart, xEnd, yEnd;
		xStart = floor(pos.x / gridSize) * gridSize;
		yStart = floor(pos.y / gridSize) * gridSize;
		xEnd = xStart + gridSize;
		yEnd = yStart + gridSize;

		maths::Vec2<float> corners[4] =
		{
			{xStart, yStart},
			{xStart, yEnd},
			{xEnd, yStart},
			{xEnd, yEnd}
		};

		for (int i = 0; i < 4; i++)
		{
			data[i] = getVectorAtPosition(seed, corners[i]);
		}
	}


	maths::Vec2<float> getVectorAtPosition(float seed, maths::Vec2<float> pos) 
	{
		float n = randomFloat::randomNumber(pos.x * pos.y - pos.x + pos.y + seed);
		return{ cos(n), sin(n) };
	}
}