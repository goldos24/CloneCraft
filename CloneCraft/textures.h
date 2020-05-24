#pragma once
#include "maths.h"

namespace textures
{
	struct FaceTexture
	{
		FaceTexture(std::string path)
		{
			this->filePath = path;

			if (!texture.loadFromFile(path))
			{
				printf("Texture not loaded!\n");
			}
		}

		std::string filePath;
		sf::Texture texture;
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

	namespace faceTextures
	{
		FaceTexture* air = new FaceTexture("resources/air.png");
		FaceTexture* stone = new FaceTexture("resources/stone.png");
		FaceTexture* dirt = new FaceTexture("resources/dirt.png");
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

	namespace blockTextures
	{
		BlockTexture* stone = new BlockTexture(faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone);
	}
}
