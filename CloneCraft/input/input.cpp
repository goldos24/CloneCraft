#include <iostream>
#include "input.h"

// KeyPressState
input::KeyPressState::KeyPressState() : pressed(false), elapsedTime(0) { }
input::KeyPressState::KeyPressState(bool pressed) : pressed(pressed), elapsedTime(0) { }

// InputManager
input::InputManager::InputManager()
{
	for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) this->addMouseButton((sf::Mouse::Button)i);
	for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++) this->addKey((sf::Keyboard::Key)i);
}

void input::InputManager::clearInput()
{
	this->inputField.resize(0);
}

void input::InputManager::getAndClearInput(std::string& inputField)
{
	inputField += this->inputField;
	this->clearInput();
}

void input::InputManager::updateEvents(sf::RenderWindow& ourWindow)
{
	// Resetting the stored values

	this->wasWindowClosed = false;
	this->wasWindowResized = false;

	this->lastScrollDelta = 0;

	// handle events
	sf::Event event;
	//std::cout << "Window pointer" << ourWindow << std::endl;
	while (ourWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->wasWindowClosed = true;
		}
		else if (event.type == sf::Event::Resized)
		{
			this->wasWindowResized = true;
			this->newSizeX = event.size.width;
			this->newSizeY = event.size.height;
		}
		else if (event.type == sf::Event::TextEntered)
		{
			if(event.text.unicode >= 32 || event.text.unicode == '\n')inputField += static_cast<char>(event.text.unicode);
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			this->lastScrollDelta = event.mouseWheelScroll.delta;
		}
	}
}

void input::InputManager::updateKeyPresses(float elapsedTime)
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

void input::InputManager::updateMouseButtonPresses()
{
	for (auto b : this->mouseButtonPressStates)
	{
		this->mouseButtonPressStates[b.first] = sf::Mouse::isButtonPressed(b.first);
	}
}

void input::InputManager::resetAllTo(bool value)
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

void input::InputManager::resetKeyTo(sf::Keyboard::Key key, bool value)
{
	this->keyPressStates[key] = value;
}

void input::InputManager::resetMouseButtonTo(sf::Mouse::Button button, bool value)
{
	this->mouseButtonPressStates[button] = value;
}

void input::InputManager::addMouseButton(sf::Mouse::Button button)
{
	this->mouseButtonPressStates[button] = false;
}

void input::InputManager::addKey(sf::Keyboard::Key key)
{
	this->keyPressStates[key] = KeyPressState(false);
}

bool input::InputManager::isKeyBeingPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool input::InputManager::isKeyStillBeingPressedAfterDelay(sf::Keyboard::Key key, float delay)
{
	if (this->keyPressStates[key].elapsedTime >= delay)
	{
		return this->isKeyBeingPressed(key);
	}
	return false;
}

bool input::InputManager::isKeyReleased(sf::Keyboard::Key key)
{
	return this->keyPressStates[key].pressed && !sf::Keyboard::isKeyPressed(key);
}

bool input::InputManager::isKeyPressed(sf::Keyboard::Key key)
{
	return !this->keyPressStates[key].pressed && sf::Keyboard::isKeyPressed(key);
}

bool input::InputManager::isMouseButtonBeingPressed(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}

bool input::InputManager::isMouseButtonReleased(sf::Mouse::Button button)
{
	return this->mouseButtonPressStates[button] && !sf::Mouse::isButtonPressed(button);
}

bool input::InputManager::isMouseButtonPressed(sf::Mouse::Button button)
{
	return !this->mouseButtonPressStates[button] && sf::Mouse::isButtonPressed(button);
}