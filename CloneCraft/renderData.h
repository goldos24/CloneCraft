#pragma once
#include <iostream>
#include "textures.h"
#include "blocks.h"
#include "maths.h"
#include "renderer.h"
#include "facePosition.h"
#include "blocks.h"

namespace renderData
{
	

	struct BlockFace
	{
		BlockFace(facePos::FacePosition position, blox::ID blockID, maths::Vec3i blockPosition)
			:
			vertices(renderer::vertices::faces + position)
		{
			blox::eliminateFalseID(blockID);
			this->position = maths::Vec3( float(blockPosition.x), float(blockPosition.y), float(blockPosition.z)) ;
			this->texture = blox::getByID(blockID).texture->getFaceTexture(position);
		}

		const renderer::FaceVertexContainer* vertices;
		textures::FaceTexture* texture;

		maths::Vec3 position;

		void render()
		{
			renderer::block::drawFace(this->texture, 0.f, vertices->addVec3f(this->position));
		}
	};

	auto makeBottomFace(blox::ID id, int x, int y, int z, bool swapSides)
	{
		auto position = !swapSides ?
			facePos::top
			:
			facePos::bottom;
		return BlockFace(position, id, maths::Vec3i(x, y, z));
	}

	auto makeFrontFace(blox::ID id, int x, int y, int z, bool swapSides)
	{
		auto position = !swapSides ?
			facePos::front
			:
			facePos::back;
		return BlockFace(position, id, maths::Vec3i(x, y, z));
	}

	auto makeLeftFace(blox::ID id, int x, int y, int z, bool swapSides)
	{
		auto position = !swapSides ?
			facePos::left
			:
			facePos::right;
		return BlockFace(position, id, maths::Vec3i(x, y, z));
	}
}