#include "game.h"
#include "../mobs/Sheep.h"

Game::Game(input::InputManager& inputManager, texStorage::TextureAtlas& texAtlas) :
	inputManager(inputManager),
	texAtlas(texAtlas)
{
	this->player.position = maths::Vec3<float>(0.f, 100.0f, 0.f);

	this->guiManager.addGui(&this->commandGui);
	this->guiManager.addUIElementToGuiWithName(&this->simpleBackgroundRect, "command");
	this->guiManager.addUIElementToGuiWithName(&this->commandInfoText, "command");
	this->guiManager.addTextFieldToGuiWithName(&this->commandTextField, "command");
	this->guiManager.addButtonToGuiWithName(&this->executeCommandButton, "command");

	this->guiManager.addGui(&this->pauseGui);
	this->guiManager.addUIElementToGuiWithName(&this->simpleBackgroundRect, "pause");
	this->guiManager.addButtonToGuiWithName(&this->optionsButton, "pause");
	this->guiManager.addButtonToGuiWithName(&this->backToGameButton, "pause");
	this->guiManager.addButtonToGuiWithName(&this->backToMainMenuButton, "pause");

	this->guiManager.addGui(&this->optionsGui);
	this->guiManager.addUIElementToGuiWithName(&this->darkerSimpleBackgroundRect, "options");
	this->guiManager.addButtonToGuiWithName(&this->backToPauseGuiButton, "options");
	this->guiManager.addButtonToGuiWithName(&this->toggleFullscreenButton, "options");
	this->guiManager.addTextFieldToGuiWithName(&this->mouseSensitivityTextInput, "options");
	this->guiManager.addUIElementToGuiWithName(&this->mouseSensitivityText, "options");
	this->guiManager.addButtonToGuiWithName(&this->mouseSensitivityOKButton, "options");

	this->guiManager.addGui(&this->mainMenu);
	this->guiManager.addTextFieldToGuiWithName(&this->worldNameTextField, "main_menu");
	this->guiManager.addButtonToGuiWithName(&this->loadWorldButton, "main_menu");
	this->guiManager.addUIElementToGuiWithName(&this->worldNameInfoText, "main_menu");
	this->guiManager.addTextFieldToGuiWithName(&this->generatedWorldSeedTextField, "main_menu");
	this->guiManager.addButtonToGuiWithName(&this->generateWorldButton, "main_menu");
	this->guiManager.addUIElementToGuiWithName(&this->seedInfoText, "main_menu");
	this->guiManager.setGuiByName("main_menu");

}

void Game::updateLoadedChunks()
{
	maths::Vec3<int> playerPositionInt = maths::Vec3<int>(int(this->player.position.x), int(this->player.position.y), int(this->player.position.z));

	if (lastChunkUpdatePosition.overlySimpleDistanceTo(playerPositionInt) > 16)
	{
		this->gameWorld.moveTo(playerPositionInt - maths::Vec3<int>(gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2));
		this->lastChunkUpdatePosition = playerPositionInt;
	}
}

void Game::updateRotation(sf::Vector2u wsize, sf::RenderWindow& window)
{
	this->windowCenter = sf::Vector2f(float(wsize.x) / 2, float(wsize.y) / 2);

	auto position = sf::Vector2f(sf::Mouse::getPosition(window));

	sf::Vector2f positionDifference = position - this->lastMousePos;

	sf::Mouse::setPosition(sf::Vector2i(windowCenter.x, windowCenter.y), window);

	this->lastMousePos = sf::Vector2f(sf::Mouse::getPosition(window));

	this->player.rotate(float(positionDifference.y), float(positionDifference.x), this->player.mouseSensitivity);
}

void Game::updatePosition(float elapsedTime)
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

void Game::runCommand(std::string cmd)
{
	std::istringstream cmdin(cmd);
	std::string command;

	cmdin >> command;

	if (command == "setblock")
	{
		int x = 0, y = 0, z = 0;
		std::string blockName;
		cmdin >> x >> y >> z >> blockName;
		this->gameWorld.setBlockID(maths::Vec3<int>(x, y, z), blox::getByName(blockName).id);
	}
	else if (command == "teleport")
	{
		float x = 0.f, y = 0.f, z = 0.f;
		cmdin >> x >> y >> z;
		this->player.position = maths::Vec3<float>(x, y, z);
	}
	else if (command == "getblock")
	{
		int x = 0, y = 0, z = 0;
		cmdin >> x >> y >> z;
		std::cout << gameWorld.getBlockID(maths::Vec3<int>(x, y, z));
	}
	else if (command == "save")
	{
		this->gameWorld.save();
		this->player.saveDataToFile(this->gameWorld.worldFileName);
	}
	else if (command == "spawn")
	{
		float x, y, z;
		cmdin >> x >> y >> z;
		auto chunk = this->gameWorld.getChunk(chunks::convertToChunkPos(maths::Vec3<float>(x, y, z)));
		auto newEntity = std::make_shared<Sheep>();
		newEntity->position = maths::Vec3<float>(x,y,z);
		chunk->entities.push_back(newEntity);
	}
	else
	{
		std::cout << "Command \"" << command << "\" wasn't found!" << std::endl;
	}
}

void Game::drawGame(sf::Vector2u wsize, sf::RenderWindow& window, sf::Clock& clock)
{
	if (this->inputManager.isKeyPressed(sf::Keyboard::F11))
	{
		this->shouldRestartClock = true;
		this->shouldUpdateWindow = true;
		this->isFullscreenEnabled = !this->isFullscreenEnabled;
	}

	if (this->shouldUpdateWindow)
	{
		std::cout << "Toggle Fullscreen" << std::endl;
		this->shouldUpdateWindow = false;
		if (this->isFullscreenEnabled)
		{
			window.create(sf::VideoMode::getFullscreenModes()[0], "Our Window", sf::Style::Fullscreen, sf::ContextSettings(32));
			window.setVerticalSyncEnabled(true);
			window.setActive(true);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);

			this->texAtlas.bind();
		}
		else 
		{
			window.create(sf::VideoMode(800, 600), "Our Window", sf::Style::Default, sf::ContextSettings(32));
			window.setVerticalSyncEnabled(true);
			window.setActive(true);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);

			this->texAtlas.bind();
		}
	}

	if (this->shouldRestartClock)
	{
		this->shouldRestartClock = false;
		clock.restart();
	}

	if (!this->guiManager.isGuiWithNameActive("main_menu"))
	{
		this->updateLoadedChunks();

		glClearColor(0.3f, 0.3f, 0.3f, 1.f);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		oldf::glu::Perspective(60, (float)wsize.x / (float)wsize.y, 0.1f, 512.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		sf::Time elapsed = clock.restart();
		float elapsedSeconds = elapsed.asSeconds();
		if (!this->guiManager.isGuiSet())
		{
			this->player.update(*this, elapsedSeconds);
			updateRotation(wsize, window);
			this->gameWorld.updateEntities(*this, elapsedSeconds);
		}

		this->manageKeys();

		int newBlockValue = int(this->selectedBlockToPlace) + this->inputManager.lastScrollDelta;
		if (newBlockValue < 0) newBlockValue = blox::enumSize - 1;
		if (newBlockValue > blox::enumSize - 1) newBlockValue = 0;
		this->selectedBlockToPlace = blox::ID(newBlockValue);

		if (!this->guiManager.isGuiSet())
		{
			if (inputManager.isMouseButtonPressed(sf::Mouse::Right))
			{
				playerWorldInteraction::setBlockInFrontOfPlayer(this->gameWorld, this->player, this->selectedBlockToPlace);
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

		this->gameWorld.markVisibleChunks(this->player.rotation);

		glBegin(GL_QUADS);      

		this->gameWorld.Render(this->texAtlas, this->player.position, this->player.rotation);

		glEnd();

		this->gameWorld.RenderEntities(this->player.position, this->player.rotation);

		updateDebugInfo();

		this->simpleBackgroundRect.scale(wsize.x, wsize.y);
		this->darkerSimpleBackgroundRect.scale(wsize.x, wsize.y);

		sf::Vector2f s1 = this->crosshairRectangle1.rectElement.getSize();
		this->crosshairRectangle1.setPosition(sf::Vector2f(wsize / 2u) - s1 / 2.f);

		sf::Vector2f s2 = this->crosshairRectangle2.rectElement.getSize();
		this->crosshairRectangle2.setPosition(sf::Vector2f(wsize / 2u) - s2 / 2.f);

		this->backToGameButton.centerOnXAxis(wsize.x, window);
		this->backToMainMenuButton.centerOnXAxis(wsize.x, window);
		this->backToPauseGuiButton.centerOnXAxis(wsize.x, window);
		this->optionsButton.centerOnXAxis(wsize.x, window);
		this->toggleFullscreenButton.centerOnXAxis(wsize.x, window);

		drawUI(window);

		if (this->guiManager.isGuiSet()) this->guiManager.drawCurrentGuiToWindow(window);
	}
	else
	{
		window.setMouseCursorVisible(true);

		sf::Time elapsed = clock.restart();
		float elapsedSeconds = elapsed.asSeconds();

		if (this->guiManager.isGuiSet())
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

		if (this->guiManager.isGuiSet()) this->guiManager.drawCurrentGuiToWindow(window);
	}
}

void Game::updateDebugInfo()
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
		<< "Looking at block: " << blockPosInFrontOfPlayer << "\n" 
		<< "Selected block: " << this->selectedBlockToPlace;
	debugInfoText.updateText(debugInfoStream.str());
}

void Game::drawUI(sf::RenderWindow& window)
{
	window.pushGLStates();
	this->debugText.drawToWindow(window);
	this->debugInfoText.drawToWindow(window);
	this->crosshairRectangle1.drawToWindow(window);
	this->crosshairRectangle2.drawToWindow(window);
	window.popGLStates();
}

void Game::manageKeys()
{
	if (this->inputManager.isKeyPressed(sf::Keyboard::Escape))
	{
		if (this->guiManager.isGuiSet())
		{
			this->guiManager.textFieldManager.clearTextFields(this->guiManager.currentGui->guiName);
			this->inputManager.resetAllTo(false);
			this->guiManager.setNoGui();
		}
		else this->guiManager.setGuiByName("pause");
	}
	else if (this->inputManager.isKeyPressed(sf::Keyboard::Tab))
	{
		if (this->guiManager.isGuiSet())
		{
			this->guiManager.textFieldManager.clearTextFields("command");
			this->inputManager.resetAllTo(false);
			this->guiManager.setNoGui();
		}
		else this->guiManager.setGuiByName("command");
	}
	
	if (!this->guiManager.isGuiSet() && this->inputManager.isKeyPressed(sf::Keyboard::R))
	{
		this->player.position = maths::Vec3<float>(0.f, 100.f, 0.f);
	}

}