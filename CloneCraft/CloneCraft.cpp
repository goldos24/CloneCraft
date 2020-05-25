#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "blocks.h"
#include "chunks.h"
#include "game.h"
#include "textures.h"


int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Our Window", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    Game game;
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
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw...

        sf::Vector2u wsize = window.getSize();

        game.drawGame(wsize, window, clock);

        GLenum err = glGetError(); 
        window.display();
    }

    // release resources...

    return 0;
}