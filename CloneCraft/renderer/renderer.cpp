#include "renderer.h"

renderer::FaceVertexContainer renderer::FaceVertexContainer::addVec3f(maths::Vec3<float> absolutePosition) const
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

void renderer::block::drawFace(const textures::FaceTexture texture, const float shading,
    FaceVertexContainer vertices, texStorage::TextureAtlas& texAtlas)
{
    auto& tex = texAtlas.textures[int(texture)];

    float xOffset = fmod(abs(vertices.x1), 1.f);
    float yOffset = fmod(abs(vertices.y1), 1.f);
    if (abs(vertices.y2 - vertices.y1) < 0.01 && abs(vertices.y4 - vertices.y1) < 0.01) {
    	yOffset = fmod(abs(vertices.z1), 1.f);
    } else if (abs(vertices.x2 - vertices.x1) < 0.01 && abs(vertices.x4 - vertices.x1) < 0.01) {
    	xOffset = fmod(abs(vertices.z1), 1.f);
    }
    glColor3f(shading, shading, shading);

    tex.setGlTexCoord2f(xOffset + getFaceSize(), yOffset + getFaceSize());
    glVertex3f(vertices.x1, vertices.y1, vertices.z1);

    tex.setGlTexCoord2f(xOffset, yOffset + getFaceSize());
    glVertex3f(vertices.x2, vertices.y2, vertices.z2);

    tex.setGlTexCoord2f(xOffset, yOffset);
    glVertex3f(vertices.x3, vertices.y3, vertices.z3);

    tex.setGlTexCoord2f(xOffset + getFaceSize(), yOffset);
    glVertex3f(vertices.x4, vertices.y4, vertices.z4);
}
