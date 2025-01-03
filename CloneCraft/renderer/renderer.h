#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include "../world/blocks.h"
#include "../textures/textureStorage.h"
#include "../textures/textures.h"
#include "../renderer/facePosition.h"
#include "../maths/maths.h"
#include "../game/voxelConstants.h"

namespace renderer
{
    struct FaceVertexContainer
    {
        float x1; float y1; float z1;
        float x2; float y2; float z2;
        float x3; float y3; float z3;
        float x4; float y4; float z4;

        FaceVertexContainer addVec3f(maths::Vec3<float> absolutePosition) const;
    };

    namespace vertices
    {
        const FaceVertexContainer faces[] = { 
            //top
                { getFaceSize(), 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f            , 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f            , 0.f, getFaceSize(), // Bottom Left Of The Quad (Top)
                getFaceSize(), 0.f, getFaceSize() }, // Bottom Right Of The Quad (Top)
            //bottom
                { getFaceSize(), 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f            , 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f            , 0.f, getFaceSize(), // Bottom Left Of The Quad (Top)
                getFaceSize(), 0.f, getFaceSize() }, // Bottom Right Of The Quad (Top)
            //front
                { getFaceSize(), 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f            , 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f            , getFaceSize(), 0.f, // Bottom Left Of The Quad (Top)
                getFaceSize(), getFaceSize(), 0.f }, // Bottom Right Of The Quad (Top)
            //back
                { getFaceSize(), 0.f, 0.f, // Top Right Of The Quad (Top)
                0.f            , 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f            , getFaceSize(), 0.f, // Bottom Left Of The Quad (Top)
                getFaceSize(), getFaceSize(), 0.f }, // Bottom Right Of The Quad (Top)
            //left
                { 0.f, 0.f, getFaceSize(), // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, getFaceSize(), 0.f, // Bottom Left Of The Quad (Top)
                0.f, getFaceSize(), getFaceSize() }, // Bottom Right Of The Quad (Top)
            //right 
                { 0.f, 0.f, getFaceSize(), // Top Right Of The Quad (Top)
                0.f, 0.f, 0.f, // Top Left Of The Quad (Top)
                0.f, getFaceSize(), 0.f, // Bottom Left Of The Quad (Top)
                0.f, getFaceSize(), getFaceSize() }, // Bottom Right Of The Quad (Top)
        };
    }

    namespace block
    {
        void drawFace(const textures::FaceTexture texture, const float shading,
            FaceVertexContainer vertices, texStorage::TextureAtlas& texAtlas);
    }
}
