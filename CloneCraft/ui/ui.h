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
#include "../maths/maths.h"

namespace ui
{
	
	namespace fonts
	{
		struct UIFont
		{
			UIFont(std::string path);

			sf::Font font;
		};

		static UIFont* dos = new UIFont("resources/fonts/dos.ttf");
	}

	struct UIElement
	{
		UIElement(std::string parentGuiName);
		
		virtual void drawToWindow(sf::RenderWindow& window) = 0;
		void setVisible(bool visible);

		bool visible;
		int x, y;
		std::string parentGuiName;
	};

	struct Text : public UIElement
	{
		Text(std::string parentGuiName);
		Text(std::string parentGuiName, std::string text, fonts::UIFont* uiFont, sf::Color textColor, int x, int y, unsigned int charSize);

		void updateText(std::string newText);
		void updatePosition(int x, int y);
		void drawToWindow(sf::RenderWindow& window);
		bool operator==(Text& other);

		sf::Text textElement;
	};

	struct Rect : public UIElement
	{
		Rect(std::string parentGuiName);
		Rect(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor);

		void setPosition(int x, int y);
		void setPosition(sf::Vector2f pos);
		void setPosition(maths::Vec2<float> pos);

		void scale(int w, int h);
		void scale(sf::Vector2f scaling);
		void scale(maths::Vec2<float> scaling);

		void drawToWindow(sf::RenderWindow& window);
		bool operator==(Rect& other);

		sf::RectangleShape rectElement;
	};

	struct Button : public UIElement
	{
		Button(std::string parentGuiName);
		Button(std::string parentGuiName, int x, int y, int w, int h,
			sf::Color fillColor, std::string buttonText, fonts::UIFont* buttonFont, sf::Color textColor, int charSize,
			std::function<void()> onClick);

		void setPosition(int x, int y);
		void setPositionByOrigin(int x, int y);
		void setXPositionByOrigin(int x);
		void setYPositionByOrigin(int y);
		void centerOnXAxis(int width, sf::RenderWindow& window);
		void updateHoverState(sf::RenderWindow& window);
		void tryCallOnClick(input::InputManager& inputManager);
		void drawToWindow(sf::RenderWindow& window);

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
		ButtonManager();
		
		void addButton(Button* button);
		void update(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName);

		std::vector<Button*> buttons;
	};

	struct TextField : UIElement
	{
		TextField(std::string parentGuiName);
		TextField(std::string parentGuiName, int x, int y, int w, int h, sf::Color fillColor, sf::Color focusedFillColor,
			fonts::UIFont* textFieldFont, sf::Color textColor, int charSize);

		void updateHoverState(sf::RenderWindow& window);
		void trySetFocused(input::InputManager& inputManager, std::vector<TextField*> otherTextFields);
		void resetText();
		void tryType(input::InputManager& inputManager);
		void drawToWindow(sf::RenderWindow& window);
		bool operator==(TextField& other);

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
		TextFieldManager();
		
		void addTextField(TextField* textField);
		void updateFocus(sf::RenderWindow& window, input::InputManager& inputManager, std::string currentGuiName);
		void updateTyping(input::InputManager& inputManager, std::string currentGuiName);
		void clearTextFields(std::string currentGuiName);
		void setFocusForAll(bool focused);

		std::vector<TextField*> textFields;
	};
}