#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace ui
{
	namespace fonts
	{
		struct UIFont
		{
			UIFont(std::string path)
			{
				this->font.loadFromFile(path);
			}

			sf::Font font;
		};

		UIFont* comicSans = new UIFont("resources/fonts/comic.ttf");
		UIFont* comicSansBold = new UIFont("resources/fonts/comicbd.ttf");
		UIFont* comicSansItalic = new UIFont("resources/fonts/comici.ttf");
		UIFont* comicSansBoldItalic = new UIFont("resources/fonts/comicz.ttf");
	}

	struct Text
	{
		Text(std::string text, fonts::UIFont* font, int x, int y, unsigned int charSize = 12u)
		{
			this->textElement = sf::Text(text, font->font, charSize);
			this->textElement.setPosition(x, y);
		}

		void updateText(std::string newText)
		{
			this->textElement.setString(newText);
		}

		void updatePosition(int x, int y)
		{
			this->textElement.setPosition(x, y);
		}

		sf::Text textElement;
	};
}