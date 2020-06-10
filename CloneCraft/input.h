#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

namespace input
{

	struct KeyPressState
	{
		KeyPressState() : pressed(false), elapsedTime(0) { }
		KeyPressState(bool pressed) : pressed(pressed), elapsedTime(0) { }

		float elapsedTime;
		bool pressed;
	};

	struct InputManager
	{
		InputManager()
		{
			for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) this->addMouseButton((sf::Mouse::Button)i);
			for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++) this->addKey((sf::Keyboard::Key)i);
		}

		void updateKeyPresses(float elapsedTime)
		{
			for (auto key : this->keyPressStates)
			{
				this->keyPressStates[key.first].pressed = sf::Keyboard::isKeyPressed(key.first);
				
				if (this->isKeyBeingPressed(key.first))
				{
					this->keyPressStates[key.first].elapsedTime += elapsedTime;
				}
				
				if (this->isKeyPressed(key.first) || !this->isKeyBeingPressed(key.first))
				{
					this->keyPressStates[key.first].elapsedTime = 0;
				}
			}
		}

		void updateMouseButtonPresses()
		{
			for (auto b : this->mouseButtonPressStates)
			{
				this->mouseButtonPressStates[b.first] = sf::Mouse::isButtonPressed(b.first);
			}
		}

		void resetAllTo(bool value)
		{
			for (auto a : this->keyPressStates)
			{
				this->keyPressStates[a.first] = value;
			}

			for (auto b : this->mouseButtonPressStates)
			{
				this->mouseButtonPressStates[b.first] = value;
			}
		}

		void resetKeyTo(sf::Keyboard::Key key, bool value)
		{
			this->keyPressStates[key] = value;
		}

		void resetMouseButtonTo(sf::Mouse::Button button, bool value)
		{
			this->mouseButtonPressStates[button] = value;
		}

		void addMouseButton(sf::Mouse::Button button)
		{
			this->mouseButtonPressStates[button] = false;
		}

		void addKey(sf::Keyboard::Key key)
		{
			this->keyPressStates[key] = KeyPressState(false);
		}

		bool isKeyBeingPressed(sf::Keyboard::Key key)
		{
			return sf::Keyboard::isKeyPressed(key);
		}

		bool isKeyStillBeingPressedAfterDelay(sf::Keyboard::Key key, float delay)
		{
			if (this->keyPressStates[key].elapsedTime >= delay)
			{
				return this->isKeyBeingPressed(key);
			}
			return false;
		}

		bool isKeyReleased(sf::Keyboard::Key key)
		{
			return this->keyPressStates[key].pressed && !sf::Keyboard::isKeyPressed(key);
		}

		bool isKeyPressed(sf::Keyboard::Key key)
		{
			return !this->keyPressStates[key].pressed && sf::Keyboard::isKeyPressed(key);
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

		std::map<sf::Keyboard::Key, KeyPressState> keyPressStates;
		std::map<sf::Mouse::Button, bool> mouseButtonPressStates;
	};
}