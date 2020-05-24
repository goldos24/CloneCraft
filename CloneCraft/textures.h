#pragma once
#include "maths.h"
#include "facePosition.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

namespace textures
{
	struct FaceTexture
	{
		FaceTexture(std::string path)
		{
			this->filePath = path;

			if (!texture.loadFromFile(path))
			{
				std::cout << "Texture not loaded!"<< " Texture Path:" << path << std::endl;
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

		auto getFaceTexture(facePos::FacePosition position)
		{
			switch (position)
			{
			case facePos::top:
				return this->top;
			case facePos::bottom:
				return this->bottom;
			case facePos::front:
				return this->front;
			case facePos::back:
				return this->back;
			case facePos::left:
				return this->left;
			case facePos::right:
				return this->right;
			default:
				break;
			}
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
		const float front = 0.95f;
		const float back = 0.75f;
		const float left = 0.9f;
		const float right = 0.8f;
	}

	float getShadingFactorByPosition(facePos::FacePosition position)
	{
		switch (position)
		{
		case facePos::top:
			return shadingFactors::top;
		case facePos::bottom:
			return shadingFactors::bottom;
		case facePos::front:
			return shadingFactors::front;
		case facePos::back:
			return shadingFactors::back;
		case facePos::left:
			return shadingFactors::left;
		case facePos::right:
			return shadingFactors::right;
		default:
			break;
		}
	}

	namespace blockTextures
	{
		BlockTexture* stone = new BlockTexture(faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone);
	}
}
