#pragma once

#include "player.h"
#include "maths.h"
#include "blocks.h"
#include "chunks.h"

namespace playerWorldInteraction
{
	void fixOutOfBoundsChunkPositionAndUpdateChunk(world::World world, chunks::Chunk& chunk, int& x, int& y, int& z)
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
			y += chunks::size;
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

		float tX = maths::cosd(rightAngleMinusRotationY) * playerReach;
		float tY = maths::sind(rightAngleMinusFixedRotationX) * playerReach;
		float tZ = maths::sind(-rightAngleMinusRotationY) * playerReach;
		
		// TODO cast ray to (tX, tY, tZ) and get nearest block
		//This would approximately work like this:
		/*	std::vector<maths::Vec3i> blockPositions = raycast::bresenham(player.position.x, player.position.y, player.position.z, tX, tY, tZ);
			maths::Vec3i targetBlockPos;
			for (maths::Vec3i pos : blockPositions)
				if (!blox::isTransparent(world.getBlock(pos).id))
				{
					targetBlockPos = pos;
					break;
				}

			fixOutOfBoundsChunkPositionAndUpdateChunk(world, chunk, targetBlockPos.x, targetBlockPos.y, targetBlockPos.z);
			tX = targetBlockPos.x;
			tY = targetBlockPos.y;
			tZ = targetBlockPos.z;
		*/

		return maths::Vec3i(int(tX), int(tY), int(tZ));
	}

	void breakBlockInFrontOfPlayer(world::World world, player::Player player)
	{
		chunks::Chunk currChunk = world.findChunkFromPlayerPosition(player.position);
		maths::Vec3i playerPosI = maths::convertFromVec3ToVec3i(world.getPlayerPositionInsideCurrentChunk(player.position));
		maths::Vec3i blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(world, player, 3) + playerPosI;

		int x = int(blockPosInFrontOfPlayer.x);
		int y = int(blockPosInFrontOfPlayer.y);
		int z = int(blockPosInFrontOfPlayer.z);

		fixOutOfBoundsChunkPositionAndUpdateChunk(world, currChunk, x, y, z);
		std::cout << x << ", " << y << ", " << z;
		currChunk.setBlock(blox::air, x, y, z);
		
		// TODO break block in currChunk and update world. This doesn't work at the time, though, because the world isn't saved.
	}
}