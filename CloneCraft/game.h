#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "oldFunctions.h"
#include "blocks.h"
#include "chunks.h"
#include "player.h"
#include "maths.h"

struct Game {
    Game() {}

    chunks::Chunk gameChunk = chunks::initFlatChunk();
    float rotation = 0.f;
    player::Player player;
    sf::Vector2f lastMousePosition;

    void updateRotation(sf::Vector2u wsize, sf::RenderWindow& window)
    {
        sf::Vector2f halfWindowSize = sf::Vector2f(float(wsize.x) / 2, float(wsize.y) / 2);

        auto position = sf::Vector2f(sf::Mouse::getPosition(window));

        this->lastMousePosition = sf::Vector2f(halfWindowSize.x, halfWindowSize.y);

        sf::Vector2f positionDifference = position - sf::Vector2f(this->lastMousePosition);

        sf::Mouse::setPosition(sf::Vector2i(halfWindowSize.x, halfWindowSize.y), window);

        this->player.rotate(float(positionDifference.y), float(positionDifference.x), 1.f);
    }

    void moveRight(float elapsedTime) {
        player.position.x += maths::cosd(-player.rotation.y) * elapsedTime;
        player.position.z -= maths::sind(-player.rotation.y) * elapsedTime;
    }

    void moveLeft(float elapsedTime) {
        player.position.x -= maths::cosd(-player.rotation.y) * elapsedTime;
        player.position.z += maths::sind(-player.rotation.y) * elapsedTime;
    }

    void moveForward(float elapsedTime) {
        player.position.x -= maths::sind(-player.rotation.y) * elapsedTime;
        player.position.z -= maths::cosd(-player.rotation.y) * elapsedTime;
    }

    void moveBackward(float elapsedTime) {
        player.position.x -= maths::sind(-player.rotation.y) * elapsedTime;
        player.position.z -= maths::cosd(-player.rotation.y) * elapsedTime;
    }

    void updatePosition()
    {
        moveForward(0.01);
    }

    void drawGame(sf::Vector2u wsize, sf::RenderWindow& window)
    {

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, wsize.x, wsize.y);
        oldf::glu::Perspective(60, (float)wsize.x / (float)wsize.y, 0.1f, 512.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        updateRotation(wsize, window);

        glRotatef(this->player.rotation.x, 1.f, 0.f, 0.f);
        glRotatef(- this-> player.rotation.y, 0.f, -1.f, 0.f);

        updatePosition();

        glTranslatef(-player.position.x, -player.position.y, -player.position.z);

        glBegin(GL_QUADS);      // Draw The Cube Using quads

        this-> gameChunk.Render();

        glEnd();
    }
};