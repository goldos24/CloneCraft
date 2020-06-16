#include "ui.h"

// UIFont
ui::fonts::UIFont::UIFont(std::string path)
{
	this->font.loadFromFile(path);
}

// UIElement
ui::UIElement::UIElement(std::string parentGuiName) : parentGuiName(parentGuiName) { }

void ui::UIElement::setVisible(bool visible)
{
	this->visible = visible;
}

// Text
ui::Text::Text(std::string parentGuiName) : UIElement(parentGuiName) { }
ui::Text::Text(std::string parentGuiName, std::string text, fonts::UIFont* uiFont, sf::Color textColor, int x, int y, unsigned int charSize)
	: UIElement(parentGuiName)
{
	this->textElement = sf::Text(text, uiFont->font, charSize);
	this->x = x;
	this->y = y;

	this->textElement.setPosition(x, y);
	this->textElement.setFillColor(textColor);
	this->visible = true;
}

void ui::Text::updateText(std::string newText)
{
	this->textElement.setString(newText);
}

void ui::Text::updatePosition(int x, int y)
{
	this->x = x;
	this->y = y;
	this->textElement.setPosition(x, y);
}

void ui::Text::drawToWindow(sf::RenderWindow& window)
{
	this->textElement.setPosition(window.mapPixelToCoords(sf::Vector2i(this->x, this->y)));
	if (this->visible) window.draw(this->textElement);
}

bool ui::Text::operator==(Text& other)
{
	return
		this->x == other.x &&
		this->y == other.y &&
		this->textElement.getFillColor() == other.textElement.getFillColor() &&
		this->textElement.getString() == other.textElement.getString() &&
		this->visible == other.visible;
}

// Rect
ui::Rect::Rect(std::string parentGuiName) : UIElement(parentGuiName) { }
ui::Rect::Rect(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor)
	: UIElement(parentGuiName)
{
	this->x = x;
	this->y = y;
	this->rectElement.setPosition(x, y);
	this->rectElement.setSize(sf::Vector2f(w, h));
	this->rectElement.setFillColor(fillColor);
	this->visible = true;
}

void ui::Rect::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	this->rectElement.setPosition(x, y);
}

void ui::Rect::setPosition(sf::Vector2f pos)
{
	this->setPosition(pos.x, pos.y);
}

void ui::Rect::setPosition(maths::Vec2<float> pos)
{
	this->setPosition(pos.x, pos.y);
}

void ui::Rect::scale(int w, int h)
{
	this->rectElement.setSize(sf::Vector2f(w, h));
}

void ui::Rect::scale(sf::Vector2f scaling)
{
	this->rectElement.setSize(sf::Vector2f(scaling.x, scaling.y));
}

void ui::Rect::scale(maths::Vec2<float> scaling)
{
	this->rectElement.setSize(sf::Vector2f(scaling.x, scaling.y));
}

void ui::Rect::drawToWindow(sf::RenderWindow& window)
{
	this->rectElement.setPosition(window.mapPixelToCoords(sf::Vector2i(this->x, this->y)));
	if (this->visible) window.draw(this->rectElement);
}

bool ui::Rect::operator==(Rect& other)
{
	return
		this->x == other.x &&
		this->y == other.y &&
		this->visible == other.visible &&
		this->rectElement.getSize() == other.rectElement.getSize() &&
		this->rectElement.getFillColor() == other.rectElement.getFillColor();
}

// Button
ui::Button::Button(std::string parentGuiName) : UIElement(parentGuiName), centerText(parentGuiName), buttonRect(parentGuiName) { }

ui::Button::Button(std::string parentGuiName, int x, int y, int w, int h,
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
	this->w = buttonRect.rectElement.getSize().x;
	this->h = buttonRect.rectElement.getSize().y;
	this->fillColor = fillColor;
	this->onClick = onClick;

	int charWidth = int(charSize / 1.7);

	this->centerText.x = this->x + this->w / 2 - (charWidth * buttonText.length()) / 2;
	this->centerText.y = this->y + this->h / 2 - charSize / 2 - 3;

	this->centerText.textElement.setPosition(this->centerText.x, this->centerText.y);
}

void ui::Button::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	this->buttonRect.setPosition(x, y);

	int charWidth = int(this->charSize / 1.7);

	this->centerText.x = this->x + this->w / 2 - (charWidth * buttonText.length()) / 2;
	this->centerText.y = this->y + this->h / 2 - charSize / 2 - 3;

	this->centerText.textElement.setPosition(this->centerText.x, this->centerText.y);
}

void ui::Button::setXPositionByOrigin(int x)
{
	this->setPosition(x - this->w / 2, this->y);
}

void ui::Button::setYPositionByOrigin(int y)
{
	this->setPosition(this->x, y - this->h / 2);
}

void ui::Button::setPositionByOrigin(int x, int y)
{
	this->setPosition(x - this->w / 2, y - this->h / 2);
}

void ui::Button::centerOnXAxis(int width, sf::RenderWindow& window)
{
	width = window.mapPixelToCoords(sf::Vector2i(width, 0)).x;
	this->setXPositionByOrigin(width / 2);
}

void ui::Button::updateHoverState(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

	this->hovered =
		correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
		correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
}

void ui::Button::tryCallOnClick(input::InputManager& inputManager)
{
	if (this->visible && this->hovered && inputManager.isMouseButtonPressed(sf::Mouse::Left))
	{
		this->onClick();
	}
}

void ui::Button::drawToWindow(sf::RenderWindow& window)
{
	if (this->visible)
	{
		window.draw(this->buttonRect.rectElement);
		window.draw(this->centerText.textElement);
	}
}

// ButtonManager
ui::ButtonManager::ButtonManager() { }

void ui::ButtonManager::addButton(Button* button)
{
	this->buttons.push_back(button);
}

void ui::ButtonManager::update(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName)
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

// TextField
ui::TextField::TextField(std::string parentGuiName) : UIElement(parentGuiName), enteredTextElement(parentGuiName), textFieldRect(parentGuiName) { }
ui::TextField::TextField(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor, sf::Color focusedFillColor,
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

void ui::TextField::updateHoverState(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f correctedMousePos = window.mapPixelToCoords(mousePos);

	this->hovered =
		correctedMousePos.x >= this->x && correctedMousePos.x <= this->x + this->w &&
		correctedMousePos.y >= this->y && correctedMousePos.y <= this->y + this->h;
}

void ui::TextField::trySetFocused(input::InputManager& inputManager, std::vector<TextField*> otherTextFields)
{
	if (this->visible && inputManager.isMouseButtonPressed(sf::Mouse::Left))
	{
		if (this->hovered)
		{
			inputManager.clearInput();
			this->focused = true;
		}
		else
		{
			this->focused = false;
		}
	}

	if (this->focused)
	{
		this->textFieldRect.rectElement.setFillColor(this->focusedFillColor);
	}
	else
	{
		this->textFieldRect.rectElement.setFillColor(this->fillColor);
	}
}

void ui::TextField::resetText()
{
	this->text = "";
	this->enteredTextElement.textElement.setString("");
}

void ui::TextField::tryType(input::InputManager& inputManager)
{
	if (this->focused)
	{
		if (inputManager.isKeyPressed(sf::Keyboard::Backspace) ||
			inputManager.isKeyStillBeingPressedAfterDelay(sf::Keyboard::Backspace, .7f))
		{
			if (this->text.size() > 0)
				this->text = this->text.substr(0, this->text.size() - 1);
			inputManager.clearInput();
		}
		else 
		{
			inputManager.getAndClearInput(this->text);
		}

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

void ui::TextField::drawToWindow(sf::RenderWindow& window)
{
	if (this->visible)
	{
		window.draw(this->textFieldRect.rectElement);
		window.draw(this->enteredTextElement.textElement);
	}
}

bool ui::TextField::operator==(TextField& other)
{
	return
		this->w == other.w &&
		this->h == other.h &&
		this->textFieldRect == other.textFieldRect;
}

// TextFieldManager
ui::TextFieldManager::TextFieldManager() { }

void ui::TextFieldManager::addTextField(TextField* textField)
{
	this->textFields.push_back(textField);
}

void ui::TextFieldManager::updateFocus(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName)
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

void ui::TextFieldManager::updateTyping(input::InputManager& inputManager, std::string currentGuiName)
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

void ui::TextFieldManager::clearTextFields(std::string currentGuiName)
{
	for (TextField* textField : this->textFields)
	{
		if (textField->parentGuiName == currentGuiName)
		{
			textField->resetText();
		}
	}
}

void ui::TextFieldManager::setFocusForAll(bool focused)
{
	for (TextField* textFieldPtr : textFields)
	{
		if (textFieldPtr != nullptr)
			textFieldPtr->focused = focused;
	}
}