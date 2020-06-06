#pragma once
#include "ui.h"

namespace gui
{
	struct BackgroundResource
	{
		BackgroundResource(sf::Color color = sf::Color::Black) : 
			backgroundColor(color)
		{ }

		BackgroundResource(sf::Image image) :
			backgroundImage(image)
		{
			this->useImage = true;
		}

		bool useImage = false;
		sf::Image backgroundImage;
		sf::Color backgroundColor;
	};

	struct Gui
	{
		Gui(std::string name) : guiName(name)
		{
			this->drawBackground = false;
		}

		Gui(std::string name, BackgroundResource background) : guiName(name)
		{
			this->background = background;
		}

		Gui(std::string name, sf::Color backgroundColor) : guiName(name)
		{
			this->background = BackgroundResource(backgroundColor);
		}

		void draw(sf::RenderWindow& window)
		{
			if (this->drawBackground) window.clear(this->background.backgroundColor);
			
			window.pushGLStates();

			if (this->background.useImage && this->drawBackground)
			{
				sf::Texture backgroundTexture;
				backgroundTexture.loadFromImage(this->background.backgroundImage);
				sf::Sprite bgSprite(backgroundTexture);
				window.draw(bgSprite);
			}

			for (auto element : uiElements)
			{
				element->drawToWindow(window);
			}
			ui::Text(this->guiName, this->guiName, ui::fonts::comicSansBoldItalic, sf::Color::Red, 1, 1, 15).drawToWindow(window);

			window.popGLStates();
		}

		void addElement(ui::UIElement* elem)
		{
			this->uiElements.push_back(elem);
		}

		std::string guiName;
		bool drawBackground = true;
		std::vector<ui::UIElement*> uiElements;
		BackgroundResource background;
	};

}