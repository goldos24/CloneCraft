#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <functional>

#include "../input/input.h"

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

		bool operator==(Text& other)
		{
			return
				this->x == other.x &&
				this->y == other.y &&
				this->textElement.getFillColor() == other.textElement.getFillColor() &&
				this->textElement.getString() == other.textElement.getString() &&
				this->visible == other.visible;
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

		void scale(int w, int h)
		{
			this->sfRectangle.setSize(sf::Vector2f(w, h));
		}

		void drawToWindow(sf::RenderWindow& window)
		{
			this->sfRectangle.setPosition(window.mapPixelToCoords(sf::Vector2i(this->x, this->y)));
			if (this->visible) window.draw(this->sfRectangle);
		}

		bool operator ==(Rect& other)
		{
			return
				this->x == other.x &&
				this->y == other.y &&
				this->visible == other.visible &&
				this->sfRectangle.getSize() == other.sfRectangle.getSize() &&
				this->sfRectangle.getFillColor() == other.sfRectangle.getFillColor();
		}

		sf::RectangleShape sfRectangle;
	};

	struct Button : public UIElement
	{
		Button(std::string parentGuiName) : UIElement(parentGuiName), centerText(parentGuiName), buttonRect(parentGuiName) { }

		Button(std::string parentGuiName, int x, int y, int w, int h,
			sf::Color fillColor, std::string buttonText, fonts::UIFont* buttonFont, sf::Color textColor, int charSize, std::function<void()> onClick) :

			buttonRect(parentGuiName, x, y, w, h, fillColor),
			centerText(parentGuiName, buttonText, buttonFont, textColor, x, y, charSize),
			UIElement(parentGuiName)
		{
			this->charSize = charSize;
			this->buttonText = buttonText;
			this->visible = true;
			this->x = x;
			this->y = y;
			this->w = buttonRect.sfRectangle.getSize().x;
			this->h = buttonRect.sfRectangle.getSize().y;
			this->fillColor = fillColor;
			this->onClick = onClick;

			int charWidth = int(charSize / 1.7);
			int fittingCharCount = int(this->w / charWidth);

			if (buttonText.length() < fittingCharCount)
			{
				this->centerText.x = this->x + this->w / 2 - (charWidth * buttonText.length()) / 2;
				this->centerText.y = this->y + this->h / 2 - charSize / 2 - 3;

				this->centerText.textElement.setPosition(this->centerText.x, this->centerText.y);
			}
		}

		void setPosition(int x, int y)
		{
			this->x = x;
			this->y = y;
			this->buttonRect.setPosition(x, y);

			int charWidth = int(this->charSize / 1.7);
			int fittingCharCount = int(this->w / charWidth);

			if (this->buttonText.length() < fittingCharCount)
			{
				this->centerText.x = this->x + this->w / 2 - (charWidth * buttonText.length()) / 2;
				this->centerText.y = this->y + this->h / 2 - charSize / 2 - 3;

				this->centerText.textElement.setPosition(this->centerText.x, this->centerText.y);
			}
		}

		void setXPositionByOrigin(int x)
		{
			this->setPosition(x - this->w / 2, this->y);
		}

		void setYPositionByOrigin(int y)
		{
			this->setPosition(this->x, y - this->h / 2);
		}

		void setPositionByOrigin(int x, int y)
		{
			this->setPosition(x - this->w / 2, y - this->h / 2);
		}

		void centerOnXAxis(int width, sf::RenderWindow& window)
		{
			width = window.mapPixelToCoords(sf::Vector2i(width, 0)).x;
			this->setXPositionByOrigin(width / 2);
		}

		void updateHoverState(sf::RenderWindow& window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

			this->hovered =
				correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
				correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
		}

		void tryCallOnClick(input::InputManager& inputManager)
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
		int charSize;
		int w;
		int h;
		std::string buttonText;
		Text centerText;
		sf::Color fillColor;
		std::function<void()> onClick;
		Rect buttonRect;
	};

	struct ButtonManager
	{
		ButtonManager() { }

		void addButton(Button* button)
		{
			this->buttons.push_back(button);
		}

		void update(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName)
		{
			for (Button* button : this->buttons)
			{
				if (button != nullptr)
					if (button->parentGuiName == currentGuiName)
					{
						button->updateHoverState(window);
						button->tryCallOnClick(inputManager);
					}
			}
		}

		std::vector<Button*> buttons;
	};

	struct TextField : UIElement
	{
		TextField(std::string parentGuiName) : UIElement(parentGuiName), enteredTextElement(parentGuiName), textFieldRect(parentGuiName) { }
		TextField(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor, sf::Color focusedFillColor,
			fonts::UIFont* textFieldFont, sf::Color textColor, int charSize) :
			textFieldRect(parentGuiName, x, y, w, h, fillColor), enteredTextElement(parentGuiName, "", textFieldFont, textColor, x, y, charSize),
			UIElement(parentGuiName)
		{
			this->charSize = charSize;
			this->visible = true;
			this->focused = false;
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->fillColor = fillColor;
			this->focusedFillColor = focusedFillColor;
		}

		void updateHoverState(sf::RenderWindow& window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

			this->hovered =
				correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
				correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
		}

		void trySetFocused(input::InputManager& inputManager, std::vector<TextField*> otherTextFields)
		{
			if (this->visible && inputManager.isMouseButtonPressed(sf::Mouse::Left))
			{
				if (this->hovered)
				{
					for (TextField* textField : otherTextFields)
					{
						textField->focused = false;
					}
					this->focused = true;
				}
				else
				{
					this->focused = false;
				}
			}

			if (this->focused)
			{
				this->textFieldRect.sfRectangle.setFillColor(this->focusedFillColor);
			}
			else
			{
				this->textFieldRect.sfRectangle.setFillColor(this->fillColor);
			}
		}

		void resetText()
		{
			this->text = "";
			this->enteredTextElement.textElement.setString("");
		}

		void tryType(input::InputManager& inputManager)
		{
			if (this->focused)
			{				
				if (inputManager.isKeyPressed(sf::Keyboard::Backspace) || 
					inputManager.isKeyStillBeingPressedAfterDelay(sf::Keyboard::Backspace, .7f))
				{
					if (this->text.size() > 0) 
						this->text = this->text.substr(0, this->text.size() - 1);
				}

				for (int i = int(sf::Keyboard::A); i <= int(sf::Keyboard::Z); ++i) // Because C programmers thinks it should bee that way.
				{
					if (inputManager.isKeyPressed(sf::Keyboard::Key(i)) ||
						inputManager.isKeyStillBeingPressedAfterDelay(sf::Keyboard::Key(i), .7f))
					{
						this->text += inputManager.isKeyBeingPressed(sf::Keyboard::LShift) ? 
							char('A' + i):
							char('a' + i);
					}
				}

				for (int i = int(sf::Keyboard::Num0); i <= int(sf::Keyboard::Num9); ++i) // Because C programmers thinks it should bee that way.
				{
					if (inputManager.isKeyPressed(sf::Keyboard::Key(i)) ||
						inputManager.isKeyStillBeingPressedAfterDelay(sf::Keyboard::Key(i), .7f))
					{
						this->text += char('0' + i - int(sf::Keyboard::Num0));
					}
				}

				if (inputManager.isKeyPressed(sf::Keyboard::Space) ||
					inputManager.isKeyStillBeingPressedAfterDelay(sf::Keyboard::Space, .7f))
					this->text += ' ';

				int charWidth = int(this->charSize / 1.7);
				int fittingCharCount = int(this->w / charWidth);
				if (text.size() > fittingCharCount)
				{
					int scrolledIndex = maths::abs<int>(fittingCharCount - text.size());
					std::string scrolledText = text.substr(scrolledIndex, text.size() - 1);
					this->enteredTextElement.textElement.setString(scrolledText);
				}
				else
				{
					this->enteredTextElement.textElement.setString(this->text);
				}
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

		bool operator ==(TextField& other)
		{
			return
				this->w == other.w &&
				this->h == other.h &&
				this->textFieldRect == other.textFieldRect;
		}

		int charSize;
		std::string text;
		bool hovered;
		bool focused;
		int w;
		int h;
		Text enteredTextElement;
		sf::Color focusedFillColor;
		sf::Color fillColor;
		Rect textFieldRect;
	};

	struct TextFieldManager
	{
		TextFieldManager() { }

		void addTextField(TextField* textField)
		{
			this->textFields.push_back(textField);
		}

		void updateFocus(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName)
		{
			for (TextField* textField : this->textFields)
			{
				if (textField != nullptr)
					if (textField->parentGuiName == currentGuiName)
					{
						textField->updateHoverState(window);
						textField->trySetFocused(inputManager, this->textFields);
					}
			}
		}

		void updateTyping(input::InputManager& inputManager, std::string currentGuiName)
		{
			for (TextField* textField : this->textFields)
			{
				if (textField != nullptr)
					if (textField->parentGuiName == currentGuiName)
					{
						textField->tryType(inputManager);
					}
			}
		}

		void clearTextFields(std::string currentGuiName)
		{
			for (TextField* textField : this->textFields)
			{
				if (textField->parentGuiName == currentGuiName)
				{
					textField->resetText();
				}
			}
		}

		void setFocusForAll(bool focused)
		{
			for (TextField* textFieldPtr : textFields)
			{
				if (textFieldPtr != nullptr)
					textFieldPtr->focused = focused;
			}
		}

		std::vector<TextField*> textFields;
	};
}