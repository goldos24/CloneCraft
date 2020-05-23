#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "blocks.h"
#include "textures.h"

namespace renderer
{
	namespace block
	{
        auto drawFace(textures::FaceTexture* texture,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            glColor3f(texture->color1.x, texture->color1.y, texture->color1.z);  // Color Green
            glNormal3f(0.f, 1.f, 0.f);
            glVertex3f(x1, y1, z1);

            glNormal3f(0.0, 1.0, 0.0);
            glColor3f(texture->color2.x, texture->color2.y, texture->color2.z);  // Color Red
            glVertex3f(x2, y2, z2);

            glNormal3f(0.0, 1.0, 0.0);
            glColor3f(texture->color3.x, texture->color3.y, texture->color3.z);  // Color Blue
            glVertex3f(x3, y3, z3);

            glNormal3f(0.f, 1.f, 0.f);
            glColor3f(texture->color4.x, texture->color4.y, texture->color4.z);  // Color Purple
            glVertex3d(x4, y4, z4);
        }

        auto drawOffsetFace(textures::FaceTexture* texture, 
            float x, float y, float z,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            drawFace(texture,
                x + x1, y + y1, z + z1,
                x + x2, y + y2, z + z2,
                x + x3, y + y3, z + z3,
                x + x4, y + y4, z + z4);
        }
            

		auto drawBottomFace(blox::ID id, int x, int y, int z) 
		{
            auto* texture = blox::getByID(id).texture->bottom;
            drawOffsetFace(
                texture,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 0.f, 1.f, // Bottom Left Of The Quad (Top)
                1.f, 0.f, 1.f // Bottom Right Of The Quad (Top)
            );
		}

        auto drawFrontFace(blox::ID id, int x, int y, int z) 
        {
            auto* texture = blox::getByID(id).texture->front;
            drawOffsetFace(
                texture,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                1.f, 1.f, 0.f // Bottom Right Of The Quad (Top)
            );
        }

        auto drawLeftFace(blox::ID id, int x, int y, int z) 
        {
            auto* texture = blox::getByID(id).texture->bottom;
            drawOffsetFace(
                texture,
                x, y, z,
                0.f, 0.f, 1.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                0.f, 1.f, 1.f // Bottom Right Of The Quad (Top)
            );
        }
	}
}