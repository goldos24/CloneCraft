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
		BlockFace(facePos::FacePosition position, blox::ID blockID, maths::Vec3<int> blockPosition);

		const renderer::FaceVertexContainer* vertices;
		const textures::FaceTexture* texture;

		maths::Vec3<float> position;

		void render();
	};

	BlockFace makeFace(blox::ID id, int x, int y, int z, bool swapSides, facePos::FacePosition facePosition);
}