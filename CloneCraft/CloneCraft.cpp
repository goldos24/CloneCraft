#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "oldFunctions.h"
#include "blocks.h"

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
    glTranslatef(0.f, -2.f, -5.f);

    glBegin(GL_QUADS);      // Draw The Cube Using quads

    glColor3f(0.0f, 1.0f, 0.0f);  // Color Green
    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(1.0f, 1.0f, -1.0f);  // Top Right Of The Quad (Top)
    glNormal3f(0.f, 1.f, 0.f);
    glColor3f(1.0f, 0.0f, 0.0f);  // Color Red
    glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Quad (Top)
    glNormal3f(0.f, 1.f, 0.f);
    glColor3f(0.0f, 0.0f, 1.0f);  // Color Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom Left Of The Quad (Top)
    glNormal3f(0.f, 1.f, 0.f);
    glColor3f(1.0f, 0.0f, 1.0f);  // Color Purple
    glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom Right Of The Quad (Top)
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