#pragma once

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
		{
			this->position = position;
			this->blockID = blockID;

			this->x = blockPosition.x;
			this->y = blockPosition.y;
			this->z = blockPosition.z;
		}

		facePos::FacePosition position;
		blox::ID blockID;

		int x, y, z;

		void render(int relativeX, int relativeY, int relativeZ)
		{
			relativeX = 0;
			relativeY = 0;
			relativeZ = 0;
			auto blockPos = maths::Vec3i(relativeX + this->x, relativeY + this->y, relativeZ + this->z);
			renderer::block::drawFace(this->position, blox::getByID(blockID).texture, blockPos);
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