#pragma once
#include "../maths/maths.h"
#include "../renderer/facePosition.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "../textures/textureStorage.h"

namespace textures
{

	sf::Image loadImageFromPath(std::string path);


	enum class FaceTexture
	{
		stone,
		dirt,
		grass_side,
		grass_top,
		wewd_side,
		wewd_top,
		leaves,

		enumSize
	};

	struct BlockTexture
	{
		BlockTexture(FaceTexture top,
			FaceTexture bottom,
			FaceTexture left,
			FaceTexture right,
			FaceTexture front,
			FaceTexture back);

		FaceTexture getFaceTexture(facePos::FacePosition position);

		FaceTexture top;
		FaceTexture bottom;
		FaceTexture left;
		FaceTexture right;
		FaceTexture front;
		FaceTexture back;
	}; 

	/*namespace faceTextures
	{
		__declspec(selectany) FaceTexture* stone = new textures::FaceTexture("resources/textures/blocks/stone.png");
		__declspec(selectany) FaceTexture* dirt = new textures::FaceTexture("resources/textures/blocks/dirt.png");
		__declspec(selectany) FaceTexture* grass_side = new textures::FaceTexture("resources/textures/blocks/grass_side.png");
		__declspec(selectany) FaceTexture* grass_top = new textures::FaceTexture("resources/textures/blocks/grass_top.png");
		__declspec(selectany) FaceTexture* wewd_side = new textures::FaceTexture("resources/textures/blocks/wewd_side.png");
		__declspec(selectany) FaceTexture* wewd_top = new textures::FaceTexture("resources/textures/blocks/wewd_top.png");
		__declspec(selectany) FaceTexture* leaves = new textures::FaceTexture("resources/textures/blocks/leaves.png");
	}*/


	static float shadingFactors[] =
	{
		1.0f,          // top
		0.7f,		   // bottom
		0.95f,		   // front
		0.75f,		   // back
		0.9f,		   // left
		0.8f		   // right
	};


	namespace blockTextures
	{
		static BlockTexture stone(FaceTexture::stone, FaceTexture::stone, FaceTexture::stone, FaceTexture::stone, FaceTexture::stone, FaceTexture::stone);
		static BlockTexture dirt(FaceTexture::dirt, FaceTexture::dirt, FaceTexture::dirt, FaceTexture::dirt, FaceTexture::dirt, FaceTexture::dirt);
		static BlockTexture grass(FaceTexture::grass_top, FaceTexture::dirt, FaceTexture::grass_side, FaceTexture::grass_side, FaceTexture::grass_side, FaceTexture::grass_side);
		static BlockTexture wewd(FaceTexture::wewd_top, FaceTexture::wewd_top, FaceTexture::wewd_side, FaceTexture::wewd_side, FaceTexture::wewd_side, FaceTexture::wewd_side);
		static BlockTexture leaves(FaceTexture::leaves, FaceTexture::leaves, FaceTexture::leaves, FaceTexture::leaves, FaceTexture::leaves, FaceTexture::leaves);
	}
}
