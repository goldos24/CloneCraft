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
#include "gui.h"
#include "playerWorldInteraction.h"
#include "input.h"
#include "playerControls.h"
#include "blockInfo.h"

struct Game {
	Game()
	{
		this->player.position = maths::Vec3<float>(0, 20, 0);

		this->guiManager.addGui(&this->emptyGui);
		this->guiManager.addGui(&this->pauseGui);
		this->guiManager.addUIElementToGuiWithName(&this->simpleBackgroundRect, "pause");
		this->guiManager.addButtonToGuiWithName(&this->testButton, "pause");
		this->guiManager.addButtonToGuiWithName(&this->optionsButton, "pause");
		this->guiManager.addButtonToGuiWithName(&this->backToGameButton, "pause");

		this->guiManager.addGui(&this->optionsGui);
		this->guiManager.addUIElementToGuiWithName(&this->darkerSimpleBackgroundRect, "options");
		this->guiManager.addButtonToGuiWithName(&this->backToPauseGuiButton, "options");
		this->guiManager.addButtonToGuiWithName(&this->saveWorldButton, "options");
		this->guiManager.addTextFieldToGuiWithName(&this->testTextField, "options");
		this->guiManager.addTextFieldToGuiWithName(&this->testTextField2, "options");
	}

	world::World gameWorld = world::World();

	player::Player player;
	maths::Vec3<int> lastChunkUpdatePosition;
	std::ostringstream debugInfoStream;

	ui::Text debugText = ui::Text("", "Debug", ui::fonts::dos, sf::Color::White, 1, 0, 17);
	ui::Text debugInfoText = ui::Text("", "", ui::fonts::dos, sf::Color::White, 1, 25, 14);

	ui::Button testButton = ui::Button("pause", 1, 190, 2, 2,
		sf::Color(0, 0, 0, 125), "Test", ui::fonts::dos, sf::Color::White, 30,
		[]() { std::cout << "Test!" << std::endl; });
	ui::Button optionsButton = ui::Button("pause", 1, 250, 2, 2,
		sf::Color(0, 0, 0, 125), "Options", ui::fonts::dos, sf::Color::White, 30,
		[this]() { this->inputManager.resetAllTo(true); this->guiManager.setGuiByName("options"); });
	ui::Button backToGameButton = ui::Button("pause", 1, 310, 2, 2,
		sf::Color(0, 0, 0, 125), "Back to game", ui::fonts::dos, sf::Color::White, 30,
		[this]() { this->inputManager.resetAllTo(false); this->guiManager.setNoGui(); });

	ui::Button backToPauseGuiButton = ui::Button("options", 1, 480, 2, 2,
		sf::Color(0, 0, 0, 125), "Back", ui::fonts::dos, sf::Color::White, 30,
		[this]() 
		{ 
			this->guiManager.textFieldManager.clearTextFields("options"); 
			this->inputManager.resetAllTo(false);
			this->guiManager.setGuiByName("pause"); 
		});
	ui::Button saveWorldButton = ui::Button("options", 1, 190, 2, 2,
		sf::Color(0, 0, 0, 125), "Save world", ui::fonts::dos, sf::Color::White, 30,
		[this]() { this->gameWorld.save(); });
	ui::TextField testTextField = ui::TextField("options", 1, 260, 420, 34, sf::Color(0, 0, 0, 125), sf::Color(255, 255, 255, 125), ui::fonts::dos, sf::Color::White, 30);
	ui::TextField testTextField2 = ui::TextField("options", 1, 300, 420, 34, sf::Color(0, 0, 0, 125), sf::Color(255, 255, 255, 125), ui::fonts::dos, sf::Color::White, 30);

	ui::Rect simpleBackgroundRect = ui::Rect("", 0, 0, 0, 0, sf::Color(0, 0, 0, 125));
	ui::Rect darkerSimpleBackgroundRect = ui::Rect("", 0, 0, 0, 0, sf::Color(0, 0, 0, 195));

	ui::Rect crosshairRectangle1 = ui::Rect("", 0, 0, 18, 2, sf::Color(255, 255, 255, 255));
	ui::Rect crosshairRectangle2 = ui::Rect("", 0, 0, 2, 18, sf::Color(255, 255, 255, 255));

	gui::Gui optionsGui = gui::Gui("options");
	gui::Gui pauseGui = gui::Gui("pause");
	gui::Gui emptyGui = gui::Gui("empty");

	input::InputManager inputManager;
	gui::GuiManager guiManager;

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
		else this->movementSpeed = 5.9f;

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

		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::W)) playerControls::moveForward(this->player, elapsedTime, this->movementSpeed);
		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::S)) playerControls::moveBackward(this->player, elapsedTime, this->movementSpeed);
		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::A)) playerControls::moveLeft(this->player, elapsedTime, this->movementSpeed);
		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::D)) playerControls::moveRight(this->player, elapsedTime, this->movementSpeed);
		if (this->inputManager.isKeyBeingPressed(sf::Keyboard::Space)) playerControls::jump(this->player, 10.f, this->gameWorld);
		playerControls::applyGravity(this->player, elapsedTime, 18.f);
		this->player.clipMovement(elapsedTime, this->gameWorld);
		this->player.applyMovement(elapsedTime);
		this->player.applyFriction(elapsedTime, friction);
	}

	void getAndRunCommand()
	{
		if (this->guiManager.isGuiSet()) return;
		this->guiManager.setGuiByName("empty");

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
		this->guiManager.setNoGui();
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
		if (!this->guiManager.isGuiSet())
		{
			updatePosition(elapsedSeconds);
			updateRotation(wsize, window);
		}

		this->manageKeys();

		if (!this->guiManager.isGuiSet())
		{
			if (inputManager.isMouseButtonPressed(sf::Mouse::Right))
			{
				//std::cout << "Right click" << std::endl;
				playerWorldInteraction::setBlockInFrontOfPlayer(this->gameWorld, this->player);
			}
			if (inputManager.isMouseButtonPressed(sf::Mouse::Left))
				playerWorldInteraction::breakBlockInFrontOfPlayer(this->gameWorld, this->player);
		}
		else if (this->guiManager.isGuiSet())
		{
			this->guiManager.buttonManager.update(window, this->inputManager, this->guiManager.currentGui->guiName);
		}

		if (this->guiManager.isGuiSet())
		{
			this->guiManager.textFieldManager.updateFocus(window, this->inputManager, this->guiManager.currentGui->guiName);
		}

		if (this->guiManager.isGuiSet())
		{
			this->guiManager.textFieldManager.updateTyping(this->inputManager, this->guiManager.currentGui->guiName);
		}

		this->inputManager.updateKeyPresses(elapsedSeconds);
		this->inputManager.updateMouseButtonPresses();

		window.setMouseCursorVisible(this->guiManager.isGuiSet());

		glRotatef(this->player.rotation.x, 1.f, 0.f, 0.f);
		glRotatef(-this->player.rotation.y, 0.f, -1.f, 0.f);
		glTranslatef(-player.position.x, -player.position.y, -player.position.z);

		glBegin(GL_QUADS);      // Draw The Cubes Using quads

		this->gameWorld.Render();

		glEnd();

		updateDebugInfo();


		float windowStretchFactor = 1; // TODO calculate

		this->simpleBackgroundRect.scale(wsize.x, wsize.y);
		this->darkerSimpleBackgroundRect.scale(wsize.x, wsize.y);

		sf::Vector2f s1 = this->crosshairRectangle1.sfRectangle.getSize();
		this->crosshairRectangle1.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s1 / 2.f));

		sf::Vector2f s2 = this->crosshairRectangle2.sfRectangle.getSize();
		this->crosshairRectangle2.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s2 / 2.f));

		drawUI(window);
	}

	void updateDebugInfo()
	{
		maths::Vec3<float> blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(this->gameWorld, this->player);
		blox::ID blockIDInFrontOfPlayer = this->gameWorld.getBlockID(blockPosInFrontOfPlayer);

		debugInfoStream.clear();
		debugInfoStream.str("");
		debugInfoStream
			<< "Chunks: " << gameWorld.chunks.size() << "\n"
			<< "Position: " << this->player.position.toString() << "\n"
			<< "Rotation: " << this->player.rotation.toString() << "\n"
			<< "Chunk position: " << gameWorld.findChunkFromPlayerPosition(this->player.position)->chunkPos.toString() << "\n"
			<< "Position in chunk: " << gameWorld.getPlayerPositionInsideCurrentChunk(this->player.position).toString() << "\n"
			<< "Looking at block with ID: " << blockIDInFrontOfPlayer << " (" << int(blockIDInFrontOfPlayer) << ")\n"
			<< "Looking at block: " << blockPosInFrontOfPlayer << "\n";
		debugInfoText.updateText(debugInfoStream.str());
	}

	void drawUI(sf::RenderWindow& window)
	{
		window.pushGLStates();
		this->debugText.drawToWindow(window);
		this->debugInfoText.drawToWindow(window);
		this->crosshairRectangle1.drawToWindow(window);
		this->crosshairRectangle2.drawToWindow(window);
		if (this->guiManager.isGuiSet()) this->guiManager.drawCurrentGuiToWindow(window);
		window.popGLStates();
	}

	void manageKeys()
	{
		if (inputManager.isKeyPressed(sf::Keyboard::Escape))
		{
			if (this->guiManager.isGuiSet())
			{
				this->guiManager.textFieldManager.clearTextFields("options");
				this->inputManager.resetAllTo(false);
				this->guiManager.setNoGui();
			}
			else this->guiManager.setGuiByName("pause");
		}
		else if (inputManager.isKeyPressed(sf::Keyboard::Tab))
		{
			this->getAndRunCommand();
		}
	}
};