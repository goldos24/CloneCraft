#pragma once
#include "ui.h"

namespace gui
{
	struct BackgroundResource
	{
		BackgroundResource(sf::Color color = sf::Color::Black) : 
			backgroundColor(color)
		{ }

		void setImage(sf::Image image)
		{
			this->backgroundImage = image;
			this->useImage = true;
		}

		bool useImage = false;
		sf::Image backgroundImage;
		sf::Color backgroundColor;
	};

	struct Gui
	{
		Gui() { }
		Gui(BackgroundResource background)
		{
			this->background = background;
		}

		Gui(sf::Color backgroundColor)
		{
			this->background = BackgroundResource(backgroundColor);
		}

		void draw(sf::RenderWindow& window)
		{
			window.clear(this->background.backgroundColor);
			
			window.pushGLStates();

			if (this->background.useImage)
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

			window.popGLStates();
		}

		void addElement(ui::UIElement* elem)
		{
			this->uiElements.push_back(elem);
		}

		std::vector<ui::UIElement*> uiElements;
		BackgroundResource background;
	};

}