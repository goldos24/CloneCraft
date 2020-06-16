#include "game.h"

Game::Game(input::InputManager& inputManager) : inputManager(inputManager)
{
	this->player.position = maths::Vec3<float>(0, 20, 0);

	this->guiManager.addGui(&this->emptyGui);
	this->guiManager.addGui(&this->pauseGui);
	this->guiManager.addUIElementToGuiWithName(&this->simpleBackgroundRect, "pause");
	this->guiManager.addButtonToGuiWithName(&this->optionsButton, "pause");
	this->guiManager.addButtonToGuiWithName(&this->backToGameButton, "pause");
	this->guiManager.addButtonToGuiWithName(&this->backToMainMenuButton, "pause");

	this->guiManager.addGui(&this->optionsGui);
	this->guiManager.addUIElementToGuiWithName(&this->darkerSimpleBackgroundRect, "options");
	this->guiManager.addButtonToGuiWithName(&this->backToPauseGuiButton, "options");
	this->guiManager.addButtonToGuiWithName(&this->saveWorldButton, "options");

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

	this->player.rotate(float(positionDifference.y), float(positionDifference.x), 1.f);
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

void Game::getAndRunCommand()
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

void Game::drawGame(sf::Vector2u wsize, sf::RenderWindow& window, sf::Clock& clock)
{
	if (!this->guiManager.isGuiWithNameActive("main_menu"))
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

		this->gameWorld.markVisibleChunks(this->player.rotation);

		glBegin(GL_QUADS);      // Draw The Cubes Using quads

		this->gameWorld.Render();

		glEnd();

		updateDebugInfo();

		float windowStretchFactor = 1; // TODO calculate

		this->simpleBackgroundRect.scale(wsize.x, wsize.y);
		this->darkerSimpleBackgroundRect.scale(wsize.x, wsize.y);

		sf::Vector2f s1 = this->crosshairRectangle1.rectElement.getSize();
		this->crosshairRectangle1.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s1 / 2.f));

		sf::Vector2f s2 = this->crosshairRectangle2.rectElement.getSize();
		this->crosshairRectangle2.setPosition(sf::Vector2f(wsize / 2u) - windowStretchFactor * (s2 / 2.f));

		this->backToGameButton.centerOnXAxis(wsize.x, window);
		this->backToMainMenuButton.centerOnXAxis(wsize.x, window);
		this->backToPauseGuiButton.centerOnXAxis(wsize.x, window);
		this->optionsButton.centerOnXAxis(wsize.x, window);
		this->saveWorldButton.centerOnXAxis(wsize.x, window);

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
		<< "Looking at block: " << blockPosInFrontOfPlayer << "\n";
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