#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/OpenGL.hpp>
#include<map>
#include<string>
#include "../maths/maths.h"

namespace texStorage
{
    struct TextureAtlas;

    struct Texture
    {
        sf::Vector2f begin;
        sf::Vector2f end;
        TextureAtlas& atlas;
        void operator=(const Texture&);

        sf::Vector2f convertTexCoord(float x, float y);

        void setGlTexCoord2f(float x, float y);

    };

	struct TextureAtlas
	{

        sf::Texture finalTexture;
        std::vector<Texture> textures;
        void bind();

        TextureAtlas();
	};
}