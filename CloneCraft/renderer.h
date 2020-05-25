#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "blocks.h"
#include "textures.h"
#include "facePosition.h"
#include "maths.h"

namespace renderer
{
    struct FaceVertexContainer
    {
        float x1; float y1; float z1;
        float x2; float y2; float z2;
        float x3; float y3; float z3;
        float x4; float y4; float z4;
    };

    auto operator+(FaceVertexContainer face, maths::Vec3i absolutePosition)
    {
        FaceVertexContainer result;
        result.x1 = face.x1 + absolutePosition.x;
        result.y1 = face.y1 + absolutePosition.y;
        result.z1 = face.z1 + absolutePosition.z;

        result.x2 = face.x2 + absolutePosition.x;
        result.y2 = face.y2 + absolutePosition.y;
        result.z2 = face.z2 + absolutePosition.z;

        result.x3 = face.x3 + absolutePosition.x;
        result.y3 = face.y3 + absolutePosition.y;
        result.z3 = face.z3 + absolutePosition.z;

        result.x4 = face.x4 + absolutePosition.x;
        result.y4 = face.y4 + absolutePosition.y;
        result.z4 = face.z4 + absolutePosition.z;
        return result;
    }

    namespace vertices
    {
        const FaceVertexContainer faces[] = {
            //top
                { 1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 0.f, 1.f, // Bottom Left Of The Quad (Top)
                1.f, 0.f, 1.f }, // Bottom Right Of The Quad (Top)
            //bottom
                { 1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 0.f, 1.f, // Bottom Left Of The Quad (Top)
                1.f, 0.f, 1.f }, // Bottom Right Of The Quad (Top)
            //front
                { 1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                1.f, 1.f, 0.f }, // Bottom Right Of The Quad (Top)
            //back
                { 1.f, 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                1.f, 1.f, 0.f }, // Bottom Right Of The Quad (Top)
            //left
                { 0.f, 0.f, 1.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                0.f, 1.f, 1.f }, // Bottom Right Of The Quad (Top)
            //right 
                { 0.f, 0.f, 1.f, // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, 1.f, 0.f, // Bottom Left Of The Quad (Top)
                0.f, 1.f, 1.f }, // Bottom Right Of The Quad (Top)
        };
    }

	namespace block
	{
        void drawFace(textures::FaceTexture* texture, const float shading,
            FaceVertexContainer vertices)
        {
            glEnd(); //TODO replace by something waaaaaaaaaaaaaaaaaaay faster

            sf::Texture::bind(texture->texture);

            glBegin(GL_QUADS);

            glTexCoord2f(1, 1);
            glVertex3f(vertices.x1, vertices.y1, vertices.z1);

            glTexCoord2f(0, 1);
            glVertex3f(vertices.x2, vertices.y2, vertices.z2);

            glTexCoord2f(0, 0);
            glVertex3f(vertices.x3, vertices.y3, vertices.z3);

            glTexCoord2f(1, 0);
            glVertex3f(vertices.x4, vertices.y4, vertices.z4);
        }

        void drawFace(facePos::FacePosition position, textures::BlockTexture* texture, maths::Vec3i blockPos)
        {
            drawFace(texture->getFaceTexture(position), textures::getShadingFactorByPosition(position), vertices::faces[position] + blockPos);
        }

        auto drawOffsetFace(textures::FaceTexture* texture, const float shading,
            float x, float y, float z, FaceVertexContainer vertices)
        {
            drawFace(texture, shading,
                { x + vertices.x1, y + vertices.y1, z + vertices.z1,
                x + vertices.x2, y + vertices.y2, z + vertices.z2,
                x + vertices.x3, y + vertices.y3, z + vertices.z3,
                x + vertices.x4, y + vertices.y4, z + vertices.z4 });
        }
            

		auto drawBottomFace(blox::ID id, int x, int y, int z, bool swapSides)
		{
            auto* texture = !swapSides ?
                blox::getByID(id).texture->top
                :
                blox::getByID(id).texture->bottom;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::top : textures::shadingFactors::bottom,
                x, y, z,
                vertices::faces[facePos::bottom]
            );
		}

        auto drawFrontFace(blox::ID id, int x, int y, int z, bool swapSides)
        {
            auto* texture = !swapSides ?
                blox::getByID(id).texture->front
                :
                blox::getByID(id).texture->back;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::front : textures::shadingFactors::back,
                x, y, z,
                vertices::faces[facePos::front]
            );
        }

        auto drawLeftFace(blox::ID id, int x, int y, int z, bool swapSides) 
        {
            auto* texture = !swapSides ? 
                blox::getByID(id).texture->left
                : 
                blox::getByID(id).texture->right;
            drawOffsetFace(
                texture, !swapSides ? textures::shadingFactors::left : textures::shadingFactors::right ,
                x, y, z,
                vertices::faces[facePos::left]
            );
        }
	}
}