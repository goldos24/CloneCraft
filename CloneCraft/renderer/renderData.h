#pragma once
#include <iostream>
#include "../textures/textures.h"
#include "../world/blocks.h"
#include "../maths/maths.h"
#include "../renderer/renderer.h"
#include "../renderer/facePosition.h"
#include "../world/blocks.h"
#include "../world/blockInfo.h"

namespace renderData
{
	

	struct BlockFace
	{
		BlockFace(facePos::FacePosition position, blox::ID blockID, maths::Vec3<int> blockPosition)
			:
			vertices(renderer::vertices::faces + position)
		{
			blox::eliminateFalseID(blockID);
			this->position = maths::Vec3<float>( float(blockPosition.x), float(blockPosition.y), float(blockPosition.z)) ;
			this->texture = (blox::getByID(blockID).texture->getFaceTexture(position));
		}

		const renderer::FaceVertexContainer* vertices;
		const textures::FaceTexture* texture;

		maths::Vec3<float> position;

		void render()
		{
			renderer::block::drawFace(this->texture, 0.f, vertices->addVec3f(this->position));
		}
	};

	BlockFace makeFace(blox::ID id, int x, int y, int z, bool swapSides, facePos::FacePosition facePosition)
	{
		auto position = !swapSides ?
			facePosition
			:
			facePos::swap(facePosition);
		return BlockFace(position, id, maths::Vec3<int>(x, y, z));
	}
}