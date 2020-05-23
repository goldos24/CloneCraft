#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "blocks.h"

namespace renderer
{
	namespace block
	{
        auto drawFace(blox::ID id, //TODO Replace ID by something else
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            glColor3f(0.0f, 1.0f, 0.0f);  // Color Green
            glNormal3f(0.f, 1.f, 0.f);
            glVertex3f(x1, y1, z1);

            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(1.0f, 0.0f, 0.0f);  // Color Red
            glVertex3f(x2, y2, z2);

            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(0.0f, 0.0f, 1.0f);  // Color Blue
            glVertex3f(x3, y3, z3);

            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(1.0f, 0.0f, 1.0f);  // Color Purple
            glVertex3d(x4, y4, z4);
        }

        auto drawOffsetFace(blox::ID id, //TODO Replace ID by something else
            float x, float y, float z,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            drawFace(id,
                x + x1, y + y1, z + z1,
                x + x2, y + y2, z + z2,
                x + x3, y + y3, z + z3,
                x + x4, y + y4, z + z4);
        }
            

		auto drawBottomFace(blox::ID id, int x, int y, int z) // TODO a better Function
		{
            drawOffsetFace(
                id,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 0.f, 1.f, // Bottom Left Of The Quad (Top)
                1.f, 0.f, 1.f // Bottom Right Of The Quad (Top)
            );
		}

        auto drawFrontFace(blox::ID id, int x, int y, int z) // TODO a better Function
        {
            drawOffsetFace(
                id,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                1.f, 1.f, 0.f // Bottom Right Of The Quad (Top)
            );
        }

        auto drawLeftFace(blox::ID id, int x, int y, int z) // TODO a better Function
        {
            drawOffsetFace(
                id,
                x, y, z,
                0.f, 0.f, 1.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                0.f, 1.f, 1.f // Bottom Right Of The Quad (Top)
            );
        }
	}
}