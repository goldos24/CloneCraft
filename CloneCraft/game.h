#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include "oldFunctions.h"
#include "blocks.h"
#include "chunks.h"
#include "player.h"
#include "maths.h"
#include "world.h"

struct Game {
    Game() 
    {
        // Put some code here if you want to
    }

    bool isPaused = false;

    world::World gameWorld = world::World();

    player::Player player;
    maths::Vec3i lastChunkUpdatePosition;

    void updateLoadedChunks()
    {
        maths::Vec3i playerPositionInt = maths::Vec3i(int(this->player.position.x), int(this->player.position.y), int(this->player.position.z));

        if (lastChunkUpdatePosition.overlySimpleDistanceTo(playerPositionInt) > 16)
        {
            this->gameWorld.moveTo(playerPositionInt - maths::Vec3i(gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2, gameWorld.chunkRenderDistance * chunks::size / 2) );
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
        player.position.x += maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
        player.position.z -= maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
    }

    void moveLeft(float elapsedTime) 
    {
        player.position.x -= maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
        player.position.z += maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
    }

    void moveForward(float elapsedTime) 
    {
        player.position.x -= maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
        player.position.z -= maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
    }

    void moveBackward(float elapsedTime) 
    {
        player.position.x += maths::sind(-player.rotation.y) * movementSpeed * elapsedTime;
        player.position.z += maths::cosd(-player.rotation.y) * movementSpeed * elapsedTime;
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

        if(!this-> isPaused) updateRotation(wsize, window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) this->isPaused = !this->isPaused; //TODO Replace by something much better
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) this->gameWorld.move(maths::Vec3i(0, 1, 0));
        glRotatef(this->player.rotation.x, 1.f, 0.f, 0.f);
        glRotatef(- this-> player.rotation.y, 0.f, -1.f, 0.f);

        sf::Time elapsed = clock.restart();
        float elapsedSeconds = elapsed.asSeconds();
        window.setTitle( "Player position :" + this->player.position.toString() + "                                   " + "FPS: " + std::to_string(int(1.f / elapsedSeconds)));

        updatePosition(elapsedSeconds);

        glTranslatef(-player.position.x, -player.position.y, -player.position.z);

        glBegin(GL_QUADS);      // Draw The Cubes Using quads

        this-> gameWorld.Render();

        glEnd();
    }
};