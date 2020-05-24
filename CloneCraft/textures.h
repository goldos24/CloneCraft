#pragma once
#include "maths.h"

namespace textures
{

	struct FaceTexture
	{
		FaceTexture(maths::Vec3 c1)
		{
			this->color1 = c1;
		}
		maths::Vec3 color1;
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
		auto grey1 = maths::Vec3(0.4f, 0.4f, 0.4f);
		auto grey2 = maths::Vec3(0.35f, 0.35f, 0.35f);
		auto grey3 = maths::Vec3(0.3f, 0.3f, 0.3f);
		auto grey4 = maths::Vec3(0.25f, 0.25f, 0.25f);
		auto green1 = maths::Vec3(0.f, 0.7f, 0.f);
		auto green2 = maths::Vec3(0.f, 0.8f, 0.f);
		auto green3 = maths::Vec3(0.f, 0.6f, 0.f);
		auto green4 = maths::Vec3(0.f, 0.5f, 0.f);
		auto brown1 = maths::Vec3(0.3f, 0.3f, 0.f);
	}

	namespace shadingFactors
	{
		const float top = 1.0f;
		const float bottom = 0.7f;
		const float left = 0.9f;
		const float right = 0.8f;
		const float front = 0.95f;
		const float back = 0.75f;
	}

	namespace faceTextures
	{
		FaceTexture* stone = new FaceTexture( colors::grey1);
		FaceTexture* grassTop = new FaceTexture(colors::green1);
		FaceTexture* grassBottom = new FaceTexture(colors::brown1);
	}

	namespace blockTextures
	{
		BlockTexture* stone = new BlockTexture(faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone);
		BlockTexture* grass = new BlockTexture(faceTextures::grassTop, faceTextures::grassBottom, faceTextures::grassBottom, faceTextures::grassBottom, faceTextures::grassBottom, faceTextures::grassBottom);
	}
}
