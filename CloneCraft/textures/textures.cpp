#include "textures.h"

sf::Image textures::loadImageFromPath(std::string path)
{
	auto image = sf::Image();
	if (!image.loadFromFile(path))
	{
		std::cout << "Couldn't load texture with path :" + path;
		image.create(16, 16); // Preventing a Segfault 
	}
	return image;
}

textures::FaceTexture::FaceTexture(std::string path)
{
	this->texture = storage.add(loadImageFromPath(path));
	this->filePath = path;
}

textures::BlockTexture::BlockTexture(FaceTexture* top,
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

textures::FaceTexture* textures::BlockTexture::getFaceTexture(facePos::FacePosition position)
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
		std::cout << "Invalid position: Segfault imminent";
		break;
	}
}

float textures::getShadingFactorByPosition(facePos::FacePosition position)
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


