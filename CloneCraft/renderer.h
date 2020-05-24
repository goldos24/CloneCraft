#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "blocks.h"
#include "textures.h"

namespace renderer
{
	namespace block
	{
        auto drawFace(textures::FaceTexture* texture, const float shading,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            auto color = texture->color1 * shading;

            glColor3f(color.x, color.y, color.z);  // Color 1
            glNormal3f(0.f, 1.f, 0.f);
            glVertex3f(x1, y1, z1);

            glVertex3f(x2, y2, z2);

            glVertex3f(x3, y3, z3);

            glVertex3d(x4, y4, z4);
        }

        auto drawOffsetFace(textures::FaceTexture* texture, const float shading,
            float x, float y, float z,
            float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3,
            float x4, float y4, float z4)
        {
            drawFace(texture, shading,
                x + x1, y + y1, z + z1,
                x + x2, y + y2, z + z2,
                x + x3, y + y3, z + z3,
                x + x4, y + y4, z + z4);
        }
            

		auto drawBottomFace(blox::ID id, int x, int y, int z, bool swapSides)
		{
            auto* texture = swapSides ?
                blox::getByID(id).texture->top
                :
                blox::getByID(id).texture->bottom;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::top : textures::shadingFactors::bottom,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 0.f, 1.f, // Bottom Left Of The Quad (Top)
                1.f, 0.f, 1.f // Bottom Right Of The Quad (Top)
            );
		}

        auto drawFrontFace(blox::ID id, int x, int y, int z, bool swapSides)
        {
            auto* texture = swapSides ?
                blox::getByID(id).texture->front
                :
                blox::getByID(id).texture->back;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::front : textures::shadingFactors::back,
                x, y, z,
                1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                1.f, 1.f, 0.f // Bottom Right Of The Quad (Top)
            );
        }

        auto drawLeftFace(blox::ID id, int x, int y, int z, bool swapSides) 
        {
            auto* texture = swapSides ? 
                blox::getByID(id).texture->left
                : 
                blox::getByID(id).texture->right;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::left : textures::shadingFactors::right ,
                x, y, z,
                0.f, 0.f, 1.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                0.f, 1.f, 1.f // Bottom Right Of The Quad (Top)
            );
        }
	}
}