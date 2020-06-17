#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "../world/blocks.h"
#include "../world/chunks.h"
#include "game.h"
#include "../textures/textures.h"


int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Our Window", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    input::InputManager* inputManager = new input::InputManager();
    Game game(*inputManager);
    window.setMouseCursorVisible(false);

    // initialize the clock
    sf::Clock clock;

    // Enable OpenGL 2D Textures and depth test
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // Initialise the texture storage
    textures::storage.makeTexture();
    textures::storage.bind();

    // run the main loop
    bool running = true;
    while (running)
    {

        sf::Vector2u wsize = window.getSize();

        // handle events
        inputManager->updateEvents(window);
        if (inputManager->wasWindowClosed)
        {
            // end the program
            running = false;
        }
        else if (inputManager->wasWindowResized)
        {
            sf::Vector2u newWSize = window.getSize();

            sf::FloatRect visibleArea(0, 0, newWSize.x, newWSize.y);
            window.setView(sf::View(visibleArea));

            // adjust the viewport when the window is resized
            glViewport(0, 0, inputManager->newSizeX, inputManager->newSizeY);
        }
        //glViewport(0, 0, wsize.x, wsize.y);

        // clear the buffers
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw...


        game.drawGame(wsize, window, clock);

        GLenum err = glGetError();
        window.display();
    }

    // release resources...
    delete inputManager;

    return 0;
}