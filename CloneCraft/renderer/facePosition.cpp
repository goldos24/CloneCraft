#include "facePosition.h"

facePos::FacePosition facePos::swap(FacePosition facePos)
{
	return FacePosition(
		!(facePos & 1) ? (facePos + 1) :
		(facePos - 1)
	);
}