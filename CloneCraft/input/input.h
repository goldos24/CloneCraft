#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include<SFML/Window.hpp>
#include <string>
#include <map>

namespace input
{

	struct KeyPressState
	{
		KeyPressState();
		KeyPressState(bool pressed);

		float elapsedTime;
		bool pressed;
	};

	struct InputManager
	{
		InputManager();

		bool wasWindowResized = false, wasWindowClosed = false;
		int newSizeX = 0, newSizeY = 0;
		std::map<sf::Keyboard::Key, KeyPressState> keyPressStates;
		std::map<sf::Mouse::Button, bool> mouseButtonPressStates;
		std::string inputField;

		void clearInput();
		void getAndClearInput(std::string& inputField);
		void updateEvents(sf::RenderWindow& ourWindow);
		void updateKeyPresses(float elapsedTime);
		void updateMouseButtonPresses();
		void resetAllTo(bool pressed);
		void resetKeyTo(sf::Keyboard::Key key, bool value);
		void resetMouseButtonTo(sf::Mouse::Button button, bool value);
		void addMouseButton(sf::Mouse::Button button);
		void addKey(sf::Keyboard::Key key);
		
		bool isKeyBeingPressed(sf::Keyboard::Key key);
		bool isKeyStillBeingPressedAfterDelay(sf::Keyboard::Key key, float delay);
		bool isKeyReleased(sf::Keyboard::Key key);
		bool isKeyPressed(sf::Keyboard::Key key);
		bool isMouseButtonBeingPressed(sf::Mouse::Button button);
		bool isMouseButtonReleased(sf::Mouse::Button button);
		bool isMouseButtonPressed(sf::Mouse::Button button);
	};
}