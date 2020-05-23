#pragma once
#include "maths.h"

namespace textures
{
	struct FaceTexture
	{
		FaceTexture(maths::Vec3 c1, maths::Vec3 c2, maths::Vec3 c3, maths::Vec3 c4)
		{
			this->color1 = c1;
			this->color1 = c2;
			this->color1 = c3;
			this->color1 = c4;
		}
		maths::Vec3 color1;
		maths::Vec3 color2;
		maths::Vec3 color3;
		maths::Vec3 color4;
	};

	struct BlockTexture
	{
		BlockTexture(FaceTexture* top,
			FaceTexture* bottom,
			FaceTexture* left,
			FaceTexture* right,
			FaceTexture* front,
			FaceTexture* back)
		{
			this->top = top;
			this->bottom = bottom;
			this->left = left;
			this->right = right;
			this->front = front;
			this->back = back;
		}
		FaceTexture* top;
		FaceTexture* bottom;
		FaceTexture* left;
		FaceTexture* right;
		FaceTexture* front;
		FaceTexture* back;
	};
}
