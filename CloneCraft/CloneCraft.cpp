#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "oldFunctions.h"
#include "blocks.h"
#include "chunks.h"
#include "game.h"


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

        glEnable(GL_DEPTH_TEST);
        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...

        sf::Vector2u wsize = window.getSize();

        game.drawGame(wsize, window);

        glPopMatrix();

        GLenum err = glGetError(); 
        window.display();
    }

    // release resources...

    return 0;
}