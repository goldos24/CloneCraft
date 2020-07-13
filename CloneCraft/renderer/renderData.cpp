#include "renderData.h"

renderData::BlockFace::BlockFace(facePos::FacePosition position, blox::ID blockID, maths::Vec3<int> blockPosition)
	:
	vertices(renderer::vertices::faces + position)
{
	blox::eliminateFalseID(blockID);
	this->position = maths::Vec3<float>(float(blockPosition.x), float(blockPosition.y), float(blockPosition.z));
	this->texture = (blox::getByID(blockID).texture->getFaceTexture(position));
	this->shading = textures::shadingFactors[position];
}

void renderData::BlockFace::render(texStorage::TextureAtlas& texAtlas)
{
	renderer::block::drawFace(this->texture, this->shading, vertices->addVec3f(this->position), texAtlas);
}

renderData::BlockFace renderData::makeFace(blox::ID id, int x, int y, int z, bool swapSides, facePos::FacePosition facePosition)
{
	auto position = !swapSides ?
		facePosition
		:
		facePos::swap(facePosition);
	return BlockFace(position, id, maths::Vec3<int>(x, y, z));
}
