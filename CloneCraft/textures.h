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

	namespace colors
	{
		const auto grey1 = maths::Vec3(0.4f, 0.4f, 0.4f);
		const auto grey2 = maths::Vec3(0.35f, 0.35f, 0.35f);
		const auto grey3 = maths::Vec3(0.3f, 0.3f, 0.3f);
		const auto grey4 = maths::Vec3(0.25f, 0.25f, 0.25f);
	}

	namespace faceTextures
	{
		const FaceTexture* stone = new FaceTexture( colors::grey1, colors::grey3, colors::grey2, colors::grey4);
	}

	namespace blockTextures
	{
		const BlockTexture* stone = new BlockTexture(faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone);
	}
}
