#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include "blocks.h"
#include "textureStorage.h"
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

        auto addVec3f(maths::Vec3 absolutePosition) const
        {
            FaceVertexContainer result;
            result.x1 = this->x1 + absolutePosition.x;
            result.y1 = this->y1 + absolutePosition.y;
            result.z1 = this->z1 + absolutePosition.z;

            result.x2 = this->x2 + absolutePosition.x;
            result.y2 = this->y2 + absolutePosition.y;
            result.z2 = this->z2 + absolutePosition.z;

            result.x3 = this->x3 + absolutePosition.x;
            result.y3 = this->y3 + absolutePosition.y;
            result.z3 = this->z3 + absolutePosition.z;

            result.x4 = this->x4 + absolutePosition.x;
            result.y4 = this->y4 + absolutePosition.y;
            result.z4 = this->z4 + absolutePosition.z;
            return result;
        }
    };

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
            textures::storage.select(texture->texture);

            textures::storage.setGlTexCoord2f(1, 1);
            glVertex3f(vertices.x1, vertices.y1, vertices.z1);

            textures::storage.setGlTexCoord2f(0, 1);
            glVertex3f(vertices.x2, vertices.y2, vertices.z2);

            textures::storage.setGlTexCoord2f(0, 0);
            glVertex3f(vertices.x3, vertices.y3, vertices.z3);

            textures::storage.setGlTexCoord2f(1, 0);
            glVertex3f(vertices.x4, vertices.y4, vertices.z4);
        }
    }
}