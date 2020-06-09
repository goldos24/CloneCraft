#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
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

		UIFont* dos = new UIFont("resources/fonts/dos.ttf");
	}

	struct UIElement
	{
		UIElement(std::string parentGuiName) : parentGuiName(parentGuiName) { }

		virtual void drawToWindow(sf::RenderWindow& window) = 0;

		void setVisible(bool visible)
		{
			this->visible = visible;
		}

		bool visible;
		int x;
		int y;
		std::string parentGuiName;
	};

	struct Text : public UIElement
	{
		Text(std::string parentGuiName) : UIElement(parentGuiName) { }
		Text(std::string parentGuiName, std::string text, fonts::UIFont* uiFont, sf::Color textColor, int x, int y, unsigned int charSize)
			: UIElement(parentGuiName)
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
			this->textElement.setPosition(window.mapPixelToCoords(sf::Vector2i(this->x, this->y)));
			if (this->visible) window.draw(this->textElement);
		}

		sf::Text textElement;
	};

	struct Rect : public UIElement
	{
		Rect(std::string parentGuiName) : UIElement(parentGuiName) { }
		Rect(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor)
			: UIElement(parentGuiName)
		{
			this->x = x;
			this->y = y;
			this->sfRectangle.setPosition(x, y);
			this->sfRectangle.setSize(sf::Vector2f(w, h));
			this->sfRectangle.setFillColor(fillColor);
			this->visible = true;
		}

		void setPosition(int x, int y)
		{
			this->x = x;
			this->y = y;
			this->sfRectangle.setPosition(x, y);
		}

		void setPosition(sf::Vector2f pos)
		{
			this->setPosition(pos.x, pos.y);
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			this->sfRectangle.setPosition(window.mapPixelToCoords(sf::Vector2i(this->x, this->y)));
			if (this->visible) window.draw(this->sfRectangle);
		}

		sf::RectangleShape sfRectangle;
	};

	struct Button : public UIElement
	{
		Button(std::string parentGuiName) : UIElement(parentGuiName), centerText(parentGuiName), buttonRect(parentGuiName) { }
		Button(std::string parentGuiName, int x, int y, int xSpacing, int ySpacing, 
			sf::Color fillColor, std::string buttonText, fonts::UIFont* buttonFont, sf::Color textColor, int charSize, std::function<void()> onClick) :
			buttonRect(parentGuiName, x, y, (int(charSize / 1.7) * buttonText.length()) + 2 * xSpacing, charSize + 2 * ySpacing, fillColor), 
			centerText(parentGuiName, buttonText, buttonFont, textColor, x + xSpacing, y - ySpacing, charSize),
			UIElement(parentGuiName)
		{
			this->visible = true;
			this->x = x;
			this->y = y;
			this->w = buttonRect.sfRectangle.getSize().x;
			this->h = buttonRect.sfRectangle.getSize().y;
			this->fillColor = fillColor;
			this->onClick = onClick;
		}

		void updateHoverState(sf::RenderWindow& window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

			this->hovered =
				correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
				correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
		}

		void tryCallOnClick(input::InputManager inputManager)
		{
			if (this->visible && this->hovered && inputManager.isMouseButtonPressed(sf::Mouse::Left))
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

		bool hovered;
		int w;
		int h;
		Text centerText;
		sf::Color fillColor;
		std::function<void()> onClick;
		Rect buttonRect;
	};

	//struct TextFieldFocusManager
	//{
	//	TextFieldFocusManager() { }

	//	void addTextField(TextField textField)
	//	{
	//		this->textFields.push_back(textField);
	//	}

	//	std::vector<TextField> textFields;
	//};

	struct TextField : UIElement
	{
		TextField(std::string parentGuiName) : UIElement(parentGuiName), enteredTextElement(parentGuiName), textFieldRect(parentGuiName) { }
		TextField(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor, fonts::UIFont* textFieldFont, sf::Color textColor, int charSize) :
			textFieldRect(parentGuiName, x, y, w, h, fillColor), enteredTextElement(parentGuiName, "", textFieldFont, textColor, x, y + h / 2, charSize),
			UIElement(parentGuiName)
		{
			this->visible = true;
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->fillColor = fillColor;
		}

		void updateHoverState(sf::RenderWindow& window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

			this->hovered =
				correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
				correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
		}

		void trySetFocused(input::InputManager inputManager)
		{
			this->focused = this->visible && this->hovered && inputManager.isMouseButtonPressed(sf::Mouse::Left);
		}

		void tryType()
		{
			if (this->focused)
			{
				// TODO type
			}
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			if (this->visible)
			{
				window.draw(this->textFieldRect.sfRectangle);
				window.draw(this->enteredTextElement.textElement);
			}
		}

		std::string text;
		bool hovered;
		bool focused;
		int w;
		int h;
		Text enteredTextElement;
		sf::Color fillColor;
		Rect textFieldRect;
	};
}