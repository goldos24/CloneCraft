#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/OpenGL.hpp>
#include "../maths/maths.h"

namespace texStorage
{
	typedef maths::Vec2<float> Texture;

	struct Storage
	{
		Storage(int textureWidth, int textureHeight, int storageWidth) : 
			textureWidth(textureWidth) , 
			textureHeight(textureHeight),
			storageWidth(storageWidth),
			finalTexture(sf::Texture())
		{
			this->contentImage.create(storageWidth, textureHeight);
		}

		auto add(sf::Image&& image)
		{
			int currentBeginX = int(this->currentTexCoordBeginX * float(storageWidth));
			auto result = Texture(currentTexCoordBeginX, 0.f);
			for (int i = 0; i < this->textureHeight; i++)
				for (int j = 0; j < this->textureWidth; j++)
				{
					auto pixel = image.getPixel(j, i);
					this->contentImage.setPixel(j + currentBeginX, i, pixel);
				}
			this->currentTexCoordBeginX += float(this->textureWidth) / float(this->storageWidth);
			return result;
		}

		auto* makeTexture()
		{
			this->finalTexture.loadFromImage(this->contentImage);
			this->finalTexture.setSmooth(false);
			return &this->finalTexture;
		}

		void bind()
		{
			sf::Texture::bind(&finalTexture);
		}

		void select(Texture texture)
		{
			this->selectedTexture = texture;
		}

		void setGlTexCoord2f(float x, float y)
		{
			glTexCoord2f(x * float(this->textureWidth) / float(this->storageWidth) + this->selectedTexture.x, y);
		}

		float currentTexCoordBeginX = 0.f;

		Texture selectedTexture;

		const int textureWidth, textureHeight, storageWidth;
		sf::Image contentImage;
		sf::Texture&& finalTexture;
	};
}