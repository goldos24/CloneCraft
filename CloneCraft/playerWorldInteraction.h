#pragma once

#include "player.h"
#include "maths.h"
#include "blocks.h"
#include "chunks.h"

namespace playerWorldInteraction
{
	void fixOutOfBoundsChunkPositionAndUpdateChunk(world::World world, std::shared_ptr<chunks::Chunk> chunk, int& x, int& y, int& z)
	{
		if (x < 0)
			x += chunks::size;
		else if (x >= chunks::size)
			x -= chunks::size;

		if (y < 0)
			y += chunks::size;
		else if (y >= chunks::size)
			y -= chunks::size;

		if (z < 0)
			z += chunks::size;
		else if (z >= chunks::size)
			z -= chunks::size;

		chunk = world.findChunkFromPlayerPosition(maths::Vec3(x, y, z));

		if (!maths::Vec3(x, y, z).isInBounds(maths::Vec3(0, 0, 0), maths::Vec3(chunks::size, chunks::size, chunks::size)))
		{
			fixOutOfBoundsChunkPositionAndUpdateChunk(world, chunk, x, y, z);
		}
	}

	maths::Vec3i getBlockPosInFrontOfPlayer(world::World world, player::Player player, float playerReach)
	{
		int fixedRotationX = player.rotation.x + 90;
		int rightAngleMinusFixedRotationX = 90 - fixedRotationX;
		int rightAngleMinusRotationY = 90 - player.rotation.y;

		float tX = maths::cosd(rightAngleMinusRotationY) * playerReach * maths::cosd(rightAngleMinusFixedRotationX);
		float tY = maths::sind(rightAngleMinusFixedRotationX) * playerReach;
		float tZ = maths::sind(-rightAngleMinusRotationY) * playerReach * maths::cosd(rightAngleMinusFixedRotationX);
		
		maths::Vec3 finalBlockPos;
		for (float i = 0.f; i < 100.f; i++)
		{
			finalBlockPos = maths::Vec3(tX, tY, tZ) * i / 100.f;
			auto blockID = world.getBlockID(finalBlockPos + player.position);
			if (blockID != blox::air)break;
		}

		return maths::convertVec3<float, int>(finalBlockPos);
	}

	void breakBlockInFrontOfPlayer(world::World world, player::Player player)
	{
		auto currChunk = world.findChunkFromPlayerPosition(player.position);
		maths::Vec3i playerPosI = maths::convertVec3<float, int>(world.getPlayerPositionInsideCurrentChunk(player.position));
		maths::Vec3i blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(world, player, 3) + playerPosI;

		int x = int(blockPosInFrontOfPlayer.x);
		int y = int(blockPosInFrontOfPlayer.y);
		int z = int(blockPosInFrontOfPlayer.z);

		fixOutOfBoundsChunkPositionAndUpdateChunk(world, currChunk, x, y, z);
		std::cout << "Block pos: " << x << ", " << y << ", " << z << std::endl;
		currChunk->setBlock(blox::air, x, y, z);
		
		// TODO break block in currChunk and update world. This doesn't work at the time, though, because the world isn't saved.
	}
}