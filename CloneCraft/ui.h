#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>

#include "input.h"

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
		Text() {}
		Text(std::string text, fonts::UIFont* uiFont, sf::Color textColor, int x, int y, unsigned int charSize)
		{
			this->textElement = sf::Text(text, uiFont->font, charSize);
			this->x = x;
			this->y = y;

			this->textElement.setPosition(x, y);
			this->textElement.setFillColor(textColor);
			this->visible = true;
		}

		void updateText(std::string newText)
		{
			this->textElement.setString(newText);
		}

		void updatePosition(int x, int y)
		{
			this->x = x;
			this->y = y;
			this->textElement.setPosition(x, y);
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			if (this->visible) window.draw(this->textElement);
		}

		void setVisible(bool visible)
		{
			this->visible = visible;
		}

		bool visible;
		int x;
		int y;
		sf::Text textElement;
	};

	struct Rect
	{
		Rect() {}
		Rect(int x, int y, int w, int h, sf::Color fillColor)
		{
			this->sfRectangle.setPosition(x, y);
			this->sfRectangle.setSize(sf::Vector2f(w, h));
			this->sfRectangle.setFillColor(fillColor);
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			window.draw(this->sfRectangle);
		}

		sf::RectangleShape sfRectangle;
	};

	struct Button
	{
		Button() {}
		Button(int x, int y, int w, int h, sf::Color fillColor, std::string buttonText, fonts::UIFont* buttonFont, sf::Color textColor, int charSize, std::function<void()> onClick) :
			buttonRect(x, y, w, h, fillColor), centerText(buttonText, buttonFont, textColor, x, y, charSize)
		{
			this->visible = true;
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->fillColor = fillColor;
			this->onClick = onClick;
		}

		void tryCallOnClick(sf::RenderWindow& window, input::InputManager inputManager)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			if (this->visible &&
				mousePos.x >= this->x && mousePos.x <= this->x + this->w &&
				mousePos.y >= this->y && mousePos.y <= this->y + this->h &&
				inputManager.isMouseButtonPressed(sf::Mouse::Left))
			{
				this->onClick();
			}
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			if (this->visible)
			{
				window.draw(this->buttonRect.sfRectangle);
				window.draw(this->centerText.textElement);
			}
		}

		void setVisible(bool visible)
		{
			this->visible = visible;
		}

		bool visible;
		int x;
		int y;
		int w;
		int h;
		Text centerText;
		sf::Color fillColor;
		std::function<void()> onClick;
		Rect buttonRect;
	};
}