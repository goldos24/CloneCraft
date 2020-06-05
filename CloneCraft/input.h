#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

namespace input
{
	struct InputManager
	{
		InputManager()
		{
			this->addMouseButton(sf::Mouse::Left);
			this->addKey(sf::Keyboard::Escape);
		}

		void update()
		{
			for (auto a : this->keyPressStates)
			{
				keyPressStates[a.first] = sf::Keyboard::isKeyPressed(a.first);
			}

			for (auto b : this->mouseButtonPressStates)
			{
				mouseButtonPressStates[b.first] = sf::Mouse::isButtonPressed(b.first);
			}
		}

		void addMouseButton(sf::Mouse::Button button)
		{
			this->mouseButtonPressStates[button] = false;
		}

		void addKey(sf::Keyboard::Key key)
		{
			this->keyPressStates[key] = false;
		}

		bool isKeyBeingPressed(sf::Keyboard::Key key)
		{
			return sf::Keyboard::isKeyPressed(key);
		}

		bool isKeyReleased(sf::Keyboard::Key key)
		{
			return this->keyPressStates[key] && !sf::Keyboard::isKeyPressed(key);
		}

		bool isKeyPressed(sf::Keyboard::Key key)
		{
			return !this->keyPressStates[key] && sf::Keyboard::isKeyPressed(key);
		}

		bool isMouseButtonBeingPressed(sf::Mouse::Button button)
		{
			return sf::Mouse::isButtonPressed(button);
		}

		bool isMouseButtonReleased(sf::Mouse::Button button)
		{
			return this->mouseButtonPressStates[button] && !sf::Mouse::isButtonPressed(button);
		}

		bool isMouseButtonPressed(sf::Mouse::Button button)
		{
			return !this->mouseButtonPressStates[button] && sf::Mouse::isButtonPressed(button);
		}

		std::map<sf::Keyboard::Key, bool> keyPressStates;
		std::map<sf::Mouse::Button, bool> mouseButtonPressStates;
	};
}