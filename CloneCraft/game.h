#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <sstream>
#include <iostream>
#include <string>
#include "oldFunctions.h"
#include "blocks.h"
#include "chunks.h"
#include "player.h"
#include "maths.h"
#include "world.h"
#include "ui.h"
#include "playerWorldInteraction.h"
#include "physics.h"
#include "input.h"

struct Game {
	Game()
	{
		// Put some code here if you want to
		this->player.position = maths::Vec3(0, 20, 0);
		this->testButton = ui::Button(1, 200, 200, 46, sf::Color(0, 0, 0, 125), "Yeet", ui::fonts::comicSans, sf::Color::Red, 30, []() { std::cout << "YEEEEEEEET!" << std::endl; });
		this->backToGameButton = ui::Button(1, 260, 200, 46, sf::Color(0, 0, 0, 125), "Back to game", ui::fonts::comicSans, sf::Color::Red, 30, [this]() { this->isPaused = false; });
	}

	bool isPaused = false;

	world::World gameWorld = world::World();

	player::Player player;
	maths::Vec3i lastChunkUpdatePosition;
	std::ostringstream debugInfoStream;

	ui::Text debugText = ui::Text("Debug", ui::fonts::comicSansBold, sf::Color::White, 1, 0, 18);
	ui::Text debugInfoText = ui::Text("", ui::fonts::comicSans, sf::Color::White, 1, 25, 13);
	ui::Button testButton;
	ui::Button backToGameButton;
	input::InputManager inputManager;

	void updateLoadedChunks()
	{
		maths::Vec3i playerPositionInt = maths::Vec3i(int(this->player.position.x), int(this->player.position.y), int(this->player.position.z));

		if (lastChunkUpdatePosition.overlySimpleDistanceTo(playerPositionInt) > 16)
		{
			this->gameWorld.moveTo(playerPositionInt - maths::Vec3i(gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2));
			this->lastChunkUpdatePosition = playerPositionInt;
		}
	}

	float movementSpeed = 6.9f;
	float rotation = 0.f;
	sf::Vector2f windowCenter;

	float lastElapsed = 0;

	void updateRotation(sf::Vector2u wsize, sf::RenderWindow& window)
	{
		sf::Vector2f halfWindowSize = sf::Vector2f(float(wsize.x) / 2, float(wsize.y) / 2);

		auto position = sf::Vector2f(sf::Mouse::getPosition(window));

		this->windowCenter = sf::Vector2f(halfWindowSize.x, halfWindowSize.y);

		sf::Vector2f positionDifference = position - sf::Vector2f(this->windowCenter);

		sf::Mouse::setPosition(sf::Vector2i(halfWindowSize.x, halfWindowSize.y), window);

		this->player.rotate(float(positionDifference.y), float(positionDifference.x), 1.f);
	}

	void moveUp(float elapsedTime)
	{
		player.position.y += movementSpeed * elapsedTime;
	}

	void moveDown(float elapsedTime)
	{
		player.position.y -= movementSpeed * elapsedTime;
	}

	void moveRight(float elapsedTime)
	{
		/*player.position.x += maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
		player.position.z -= maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;*/
		physixx::applyAcceleration(this->player, elapsedTime, maths::Vec3(maths::cosd(-player.rotation.y) * movementSpeed, 0.f, -maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveLeft(float elapsedTime)
	{
		/*player.position.x -= maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
		player.position.z += maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;*/

		physixx::applyAcceleration(this->player, elapsedTime, maths::Vec3(-maths::cosd(-player.rotation.y) * movementSpeed, 0.f, maths::sind(-player.rotation.y) * movementSpeed));
	}

	void moveForward(float elapsedTime)
	{
		/*player.position.x -= maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
		player.position.z -= maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;*/
		physixx::applyAcceleration(this->player, elapsedTime, maths::Vec3(-maths::sind(-player.rotation.y) * movementSpeed, 0.f, -maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void moveBackward(float elapsedTime)
	{
		/*player.position.x += maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
		player.position.z += maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;*/

		physixx::applyAcceleration(this->player, elapsedTime, maths::Vec3(maths::sind(-player.rotation.y) * movementSpeed, 0.f, maths::cosd(-player.rotation.y) * movementSpeed));
	}

	void updatePosition(float elapsedTime)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) movementSpeed = 24.f;
		else movementSpeed = 6.9f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveForward(elapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveBackward(elapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveLeft(elapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveRight(elapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) moveUp(elapsedTime);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) moveDown(elapsedTime);
		physixx::applyMovement(this->player, elapsedTime);
		physixx::applyFriction(this->player, elapsedTime, 1.f);
	}

	void getAndRunCommand()
	{
		std::string command;
		std::cin >> command;
		if (command == "setblock")
		{
			int x, y, z;
			std::string blockName;
			std::cin >> x >> y >> z >> blockName;
			this->gameWorld.setBlockID(maths::Vec3i(x, y, z), blox::getByName(blockName).id);
		}
	}

	void drawGame(sf::Vector2u wsize, sf::RenderWindow& window, sf::Clock& clock)
	{
		this->updateLoadedChunks();

		glClearColor(0.3f, 0.3f, 0.3f, 1.f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		oldf::glu::Perspective(60, (float)wsize.x / (float)wsize.y, 0.1f, 512.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (!this->isPaused && inputManager.isMouseButtonPressed(sf::Mouse::Left)) 
			playerWorldInteraction::breakBlockInFrontOfPlayer(this->gameWorld, this->player);
		this->manageKeys();
		this->inputManager.update();

		window.setMouseCursorVisible(this->isPaused);
		this->testButton.setVisible(this->isPaused);
		this->backToGameButton.setVisible(this->isPaused);

		if (!this->isPaused)
		{
			updateRotation(wsize, window);
			sf::Time elapsed = clock.restart();
			float elapsedSeconds = elapsed.asSeconds();

			updatePosition(elapsedSeconds);
		}

		glRotatef(this->player.rotation.x, 1.f, 0.f, 0.f);
		glRotatef(-this->player.rotation.y, 0.f, -1.f, 0.f);
		glTranslatef(-player.position.x, -player.position.y, -player.position.z);

		glBegin(GL_QUADS);      // Draw The Cubes Using quads

		this->gameWorld.Render();

		glEnd();
		updateDebugInfo();

		drawUI(window);
	}

	void updateDebugInfo()
	{
		debugInfoStream.clear();
		debugInfoStream.str("");
		debugInfoStream
			<< "Chunks: " << gameWorld.chunks.size() << "\n"
			<< "Position: " << this->player.position.toString() << "\n"
			<< "Rotation: " << this->player.rotation.toString() << "\n"
			<< "Chunk position: " << gameWorld.findChunkFromPlayerPosition(this->player.position)->chunkPos.toString() << "\n"
			<< "Position in chunk: " << gameWorld.getPlayerPositionInsideCurrentChunk(this->player.position).toString() << "\n"
			<< "Block pos in front of player inside current chunk: " << (playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player, 3) + maths::convertFromVec3ToVec3i(gameWorld.getPlayerPositionInsideCurrentChunk(this->player.position))).toString() << "\n"
			<< "Looking at block with ID:" << this->gameWorld.getBlockID(playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player, 3) + maths::convertVec3<float, int>(this->player.position)) << "\n"
			<< "Looking at block :" << (playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player, 3) + maths::convertVec3<float, int>(this->player.position)) << "\n";
		debugInfoText.updateText(debugInfoStream.str());
	}

	void drawUI(sf::RenderWindow& window)
	{
		window.pushGLStates();
		debugText.drawToWindow(window);
		debugInfoText.drawToWindow(window);
		drawAndUpdateButton(this->testButton, window);
		drawAndUpdateButton(this->backToGameButton, window);
		window.popGLStates();
	}

	void drawAndUpdateButton(ui::Button button, sf::RenderWindow& window)
	{
		button.drawToWindow(window);
		button.tryCallOnClick(window, this->inputManager);
	}

	void manageKeys()
	{
		if (inputManager.isKeyPressed(sf::Keyboard::Escape))
		{
			this->isPaused = !this->isPaused; //TODO Replace by something much better
		}
		else if (inputManager.isKeyPressed(sf::Keyboard::Tab))
		{
			this->getAndRunCommand();
		}
	}
};