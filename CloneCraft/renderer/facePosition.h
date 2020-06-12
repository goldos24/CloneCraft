#pragma once
#include "../maths/maths.h"

namespace facePos
{
	const int cubeSurfaceAmount = 6; 

	enum FacePosition : maths::uint8  //DON'T EVER DARE TO CHANGE THE ORDER OF THESE NAMES
	{
		top = 0,
		bottom = 1,
		front = 2,
		back = 3,
		left = 4,
		right = 5
	};

	FacePosition swap(FacePosition facePos)
	{
		return FacePosition(
			!(facePos & 1) ? (facePos + 1) :
			(facePos - 1)
		);
	}
}