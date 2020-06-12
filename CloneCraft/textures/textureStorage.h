#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/OpenGL.hpp>
#include "../maths/maths.h"

namespace texStorage
{
	typedef maths::Vec2<float> Texture;

	struct Storage
	{
		Storage(int textureWidth, int textureHeight, int storageWidth);

		Texture add(sf::Image&& image);

		sf::Texture* makeTexture();

		void bind();

		void select(Texture texture);

		void setGlTexCoord2f(float x, float y);

		float currentTexCoordBeginX = 0.f;

		Texture selectedTexture;

		const int textureWidth, textureHeight, storageWidth;
		sf::Image contentImage;
		sf::Texture&& finalTexture;
	};
}