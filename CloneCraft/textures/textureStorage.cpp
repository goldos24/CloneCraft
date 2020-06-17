#include <cmath>
#include "textureStorage.h"

void convertColorToSrgb(sf::Color& color) // Necessary for not having too depressingly dark colors
{
	color.r = sqrt(int(color.r) * 256);
	color.g = sqrt(int(color.g) * 256);
	color.b = sqrt(int(color.b) * 256);
}

texStorage::Storage::Storage(int textureWidth, int textureHeight, int storageWidth) :
	textureWidth(textureWidth),
	textureHeight(textureHeight),
	storageWidth(storageWidth),
	finalTexture(sf::Texture())
{
	this->contentImage.create(storageWidth, textureHeight);
}

texStorage::Texture texStorage::Storage::add(sf::Image&& image)
{
	int currentBeginX = int(this->currentTexCoordBeginX * float(storageWidth));
	auto result = Texture(currentTexCoordBeginX, 0.f);
	for (int i = 0; i < this->textureHeight; i++)
		for (int j = 0; j < this->textureWidth; j++)
		{
			auto pixel = image.getPixel(j, i);
			//convertColorToSrgb(pixel);
			this->contentImage.setPixel(j + currentBeginX, i, pixel);
		}
	this->currentTexCoordBeginX += float(this->textureWidth) / float(this->storageWidth);
	return result;
}

sf::Texture* texStorage::Storage::makeTexture()
{
	this->finalTexture.loadFromImage(this->contentImage);
	this->finalTexture.setSmooth(false);
	this->finalTexture.setSrgb(false);
	return &this->finalTexture;
}

void texStorage::Storage::bind()
{
	sf::Texture::bind(&finalTexture);
}

void texStorage::Storage::select(Texture texture)
{
	this->selectedTexture = texture;
}

void texStorage::Storage::setGlTexCoord2f(float x, float y)
{
	glTexCoord2f(x * float(this->textureWidth) / float(this->storageWidth) + this->selectedTexture.x, y);
}