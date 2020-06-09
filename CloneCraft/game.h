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
#include "gui.h"
#include "playerWorldInteraction.h"
#include "input.h"
#include "playerControls.h"
#include "blockInfo.h"

struct Game {
	Game()
	{
		this->player.position = maths::Vec3<float>(0, 20, 0);

		this->pauseGui.addElement(&this->simpleBackgroundRect);
		this->addButton(this->pauseGui, this->testButton);
		this->addButton(this->pauseGui, this->optionsButton);
		this->addButton(this->pauseGui, this->backToGameButton);

		this->optionsGui.addElement(&this->darkerSimpleBackgroundRect);
		this->addButton(this->optionsGui, this->backToPauseGuiButton);
		this->addButton(this->optionsGui, this->saveWorldButton);
	}

	void addButton(gui::Gui& gui, ui::Button& button)
	{
		gui.addElement(&button);
		buttons.push_back(button);
	}

	std::vector<ui::Button> buttons;

	world::World gameWorld = world::World();

	player::Player player;
	maths::Vec3<int> lastChunkUpdatePosition;
	std::ostringstream debugInfoStream;

	ui::Text debugText = ui::Text("", "Debug", ui::fonts::comicSansBold, sf::Color::White, 1, 0, 18);
	ui::Text debugInfoText = ui::Text("", "", ui::fonts::comicSans, sf::Color::White, 1, 25, 13);

	ui::Button testButton = ui::Button("pause", 1, 190, 200, 46,
		sf::Color(0, 0, 0, 125), "Test", ui::fonts::comicSans, sf::Color::White, 30,
		[]() { std::cout << "Test!" << std::endl; });
	ui::Button optionsButton = ui::Button("pause", 1, 250, 200, 46,
		sf::Color(0, 0, 0, 125), "Options", ui::fonts::comicSans, sf::Color::White, 30,
		[this]() { this->currentGuiPtr = &this->optionsGui; });
	ui::Button backToGameButton = ui::Button("pause", 1, 310, 200, 46,
		sf::Color(0, 0, 0, 125), "Back to game", ui::fonts::comicSans, sf::Color::White, 30,
		[this]() { this->currentGuiPtr = nullptr; });
	ui::Button backToPauseGuiButton = ui::Button("options", 1, 480, 200, 46,
		sf::Color(0, 0, 0, 125), "Back", ui::fonts::comicSans, sf::Color::White, 30,
		[this]() { this->currentGuiPtr = &this->pauseGui; });
	ui::Button saveWorldButton = ui::Button("options", 1, 190, 200, 46,
		sf::Color(0, 0, 0, 125), "Save world", ui::fonts::comicSans, sf::Color::White, 30,
		[this]() { this->gameWorld.save(); });

	ui::Rect simpleBackgroundRect = ui::Rect("", 0, 0, 0, 0, sf::Color(0, 0, 0, 125));
	ui::Rect darkerSimpleBackgroundRect = ui::Rect("", 0, 0, 0, 0, sf::Color(0, 0, 0, 195));

	ui::Rect crosshairRectangle1 = ui::Rect("", 0, 0, 18, 2, sf::Color(255, 255, 255, 255));
	ui::Rect crosshairRectangle2 = ui::Rect("", 0, 0, 2, 18, sf::Color(255, 255, 255, 255));

	gui::Gui optionsGui = gui::Gui("options");
	gui::Gui pauseGui = gui::Gui("pause");
	gui::Gui emptyGui = gui::Gui("empty");
	gui::Gui* currentGuiPtr = nullptr;

	input::InputManager inputManager;

	void updateLoadedChunks()
	{
		maths::Vec3<int> playerPositionInt = maths::Vec3<int>(int(this->player.position.x), int(this->player.position.y), int(this->player.position.z));

		if (lastChunkUpdatePosition.overlySimpleDistanceTo(playerPositionInt) > 16)
		{
			this->gameWorld.moveTo(playerPositionInt - maths::Vec3<int>(gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2));
			this->lastChunkUpdatePosition = playerPositionInt;
		}
	}

	float movementSpeed = 6.9f;
	float rotation = 0.f;
	float friction = 1.f;
	sf::Vector2f windowCenter;

	void updateRotation(sf::Vector2u wsize, sf::RenderWindow& window)
	{
		sf::Vector2f halfWindowSize = sf::Vector2f(float(wsize.x) / 2, float(wsize.y) / 2);

		auto position = sf::Vector2f(sf::Mouse::getPosition(window));

		this->windowCenter = sf::Vector2f(halfWindowSize.x, halfWindowSize.y);

		sf::Vector2f positionDifference = position - sf::Vector2f(this->windowCenter);

		sf::Mouse::setPosition(sf::Vector2i(halfWindowSize.x, halfWindowSize.y), window);

		this->player.rotate(float(positionDifference.y), float(positionDifference.x), 1.f);
	}

	void updatePosition(float elapsedTime)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) this->movementSpeed = 24.f;
		else this->movementSpeed = 6.9f;

		if (player.isStandingOnASurface(this->gameWorld))
		{
			movementSpeed *= 5.f;
			this->friction = 10.f;
		}
		else
		{
			this->friction = 1.f;
			this->movementSpeed = 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playerControls::moveForward(this->player, elapsedTime, this->movementSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerControls::moveBackward(this->player, elapsedTime, this->movementSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playerControls::moveLeft(this->player, elapsedTime, this->movementSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerControls::moveRight(this->player, elapsedTime, this->movementSpeed);
		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::Space)) playerControls::jump(this->player, 10.f, this->gameWorld);
		playerControls::applyGravity(this->player, elapsedTime, 18.f);
		this->player.clipMovement(elapsedTime, this->gameWorld);
		this->player.applyMovement(elapsedTime);
		this->player.applyFriction(elapsedTime, friction);
	}

	void getAndRunCommand()
	{
		if (this->currentGuiPtr != nullptr) return;
		this->currentGuiPtr = &this->emptyGui;

		std::string command;
		std::cin >> command;
		if (command == "setblock")
		{
			int x, y, z;
			std::string blockName;
			std::cin >> x >> y >> z >> blockName;
			this->gameWorld.setBlockID(maths::Vec3<int>(x, y, z), blox::getByName(blockName).id);
		}
		else if (command == "teleport")
		{
			float x, y, z;
			std::cin >> x >> y >> z;
			this->player.position = maths::Vec3<float>(x, y, z);
		}
		else if (command == "getblock")
		{
			int x, y, z;
			std::cin >> x >> y >> z;
			std::cout << gameWorld.getBlockID(maths::Vec3<int>(x, y, z));
		}
		else if (command == "save")
		{
			this->gameWorld.save();
		}
		this->currentGuiPtr = nullptr;
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


		sf::Time elapsed = clock.restart();
		float elapsedSeconds = elapsed.asSeconds();
		if (this->currentGuiPtr == nullptr)
		{
			updatePosition(elapsedSeconds);
			updateRotation(wsize, window);
		}

		this->manageKeys();
		if (this->currentGuiPtr == nullptr)
		{
			if (inputManager.isMouseButtonPressed(sf::Mouse::Right))
			{
				std::cout << "Right click" << std::endl;
				playerWorldInteraction::setBlockInFrontOfPlayer(this->gameWorld, this->player);
			}
			if (inputManager.isMouseButtonPressed(sf::Mouse::Left))
				playerWorldInteraction::breakBlockInFrontOfPlayer(this->gameWorld, this->player);
		}

		for (ui::Button button : this->buttons)
		{
			if (this->currentGuiPtr)
				if (this->currentGuiPtr->guiName == button.parentGuiName)
				{
					button.updateHoverState(window);
					button.tryCallOnClick(this->inputManager);
				}
		}

		this->inputManager.update();

		window.setMouseCursorVisible(this->currentGuiPtr);
		this->testButton.setVisible(this->currentGuiPtr);
		this->backToGameButton.setVisible(this->currentGuiPtr);

		glRotatef(this->player.rotation.x, 1.f, 0.f, 0.f);
		glRotatef(-this->player.rotation.y, 0.f, -1.f, 0.f);
		glTranslatef(-player.position.x, -player.position.y, -player.position.z);

		glBegin(GL_QUADS);      // Draw The Cubes Using quads

		this->gameWorld.Render();

		glEnd();

		updateDebugInfo();


		float windowStretchFactor = 1; // TODO calculate

		this->simpleBackgroundRect.sfRectangle.setSize(sf::Vector2f(wsize.x, wsize.y));
		this->darkerSimpleBackgroundRect.sfRectangle.setSize(sf::Vector2f(wsize.x, wsize.y));

		sf::Vector2f s1 = this->crosshairRectangle1.sfRectangle.getSize();
		this->crosshairRectangle1.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s1 / 2.f));

		sf::Vector2f s2 = this->crosshairRectangle2.sfRectangle.getSize();
		this->crosshairRectangle2.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s2 / 2.f));
		
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
			<< "Looking at block with ID: " << this->gameWorld.getBlockID(playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player)) << "\n"
			<< "Looking at block: " << (playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player)) << "\n";
		debugInfoText.updateText(debugInfoStream.str());
	}

	void drawUI(sf::RenderWindow& window)
	{
		window.pushGLStates();
		this->debugText.drawToWindow(window);
		this->debugInfoText.drawToWindow(window);
		this->crosshairRectangle1.drawToWindow(window);
		this->crosshairRectangle2.drawToWindow(window);
		if (this->currentGuiPtr) this->currentGuiPtr->draw(window);
		window.popGLStates();
	}

	void manageKeys()
	{
		if (inputManager.isKeyPressed(sf::Keyboard::Escape))
		{
			this->currentGuiPtr = !(this->currentGuiPtr) ? &this->pauseGui : nullptr;
		}
		else if (inputManager.isKeyPressed(sf::Keyboard::Tab))
		{
			this->getAndRunCommand();
		}

	}
};