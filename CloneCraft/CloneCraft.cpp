#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "oldFunctions.h"
#include "blocks.h"
#include "chunks.h"

chunks::Chunk gameChunk = chunks::initFlatChunk();

void drawGame(sf::Vector2u wsize)
{

    glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, wsize.x, wsize.y);
    oldf::glu::Perspective(60, (float)wsize.x / (float)wsize.y, 0.1f, 512.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(-15.f, -5.f, -30.f);

    glBegin(GL_QUADS);      // Draw The Cube Using quads

    gameChunk.Render();
    
    glEnd();
}

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Our Window", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...

        sf::Vector2u wsize = window.getSize();

        drawGame(wsize);

        glPopMatrix();

        GLenum err = glGetError(); 
        window.display();
    }

    // release resources...

    return 0;
}