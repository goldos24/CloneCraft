#pragma once
#include "../maths/maths.h"
#include "../renderer/facePosition.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "../textures/textureStorage.h"

namespace textures
{

	__declspec(selectany) texStorage::Storage storage(16, 16, 256);

	sf::Image loadImageFromPath(std::string path);

	struct FaceTexture
	{
		FaceTexture(std::string path);
		texStorage::Texture texture;
		std::string filePath;
		
	};

	struct BlockTexture
	{
		BlockTexture(FaceTexture* top,
			FaceTexture* bottom,
			FaceTexture* left,
			FaceTexture* right,
			FaceTexture* front,
			FaceTexture* back);

		FaceTexture* getFaceTexture(facePos::FacePosition position);

		FaceTexture* top;
		FaceTexture* bottom;
		FaceTexture* left;
		FaceTexture* right;
		FaceTexture* front;
		FaceTexture* back;
	}; 

	namespace faceTextures
	{
		__declspec(selectany) FaceTexture* stone = new textures::FaceTexture("resources/textures/blocks/stone.png");
		__declspec(selectany) FaceTexture* dirt = new textures::FaceTexture("resources/textures/blocks/dirt.png");
		__declspec(selectany) FaceTexture* grass_side = new textures::FaceTexture("resources/textures/blocks/grass_side.png");
		__declspec(selectany) FaceTexture* grass_top = new textures::FaceTexture("resources/textures/blocks/grass_top.png");
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

	float getShadingFactorByPosition(facePos::FacePosition position);

	namespace blockTextures
	{
		__declspec(selectany) BlockTexture* stone = new textures::BlockTexture(faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone, faceTextures::stone);
		__declspec(selectany) BlockTexture* dirt = new textures::BlockTexture(faceTextures::dirt, faceTextures::dirt, faceTextures::dirt, faceTextures::dirt, faceTextures::dirt, faceTextures::dirt);
		__declspec(selectany) BlockTexture* grass = new textures::BlockTexture(faceTextures::grass_top, faceTextures::dirt, faceTextures::grass_side, faceTextures::grass_side, faceTextures::grass_side, faceTextures::grass_side);
	}
}
