#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "blocks.h"

namespace renderer
{
	namespace block
	{
		auto drawBottomFace(blox::ID id, int x, int y, int z) // TODO a better Function
		{
            glColor3f(0.0f, 1.0f, 0.0f);  // Color Green
            glNormal3f(0.f, 1.f, 0.f);
            glVertex3f( float(x) + 1.0f, float(y) + 0.0f, float(z) + -1.0f);  // Top Right Of The Quad (Top)
            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(1.0f, 0.0f, 0.0f);  // Color Red
            glVertex3f(float(x) + -1.0f, float(y) + 0.0f, float(z) + -1.0f);  // Top Left Of The Quad (Top)
            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(0.0f, 0.0f, 1.0f);  // Color Blue
            glVertex3f(float(x) + -1.0f, float(y) + 0.0f, float(z) + 1.0f);  // Bottom Left Of The Quad (Top)
            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(1.0f, 0.0f, 1.0f);  // Color Purple
            glVertex3f(float(x) + 1.0f, float(y) + 0.0f, float(z) + 1.0f);  // Bottom Right Of The Quad (Top)
		}
	}
}