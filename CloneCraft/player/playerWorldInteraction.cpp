#include "playerWorldInteraction.h"
#include "../game/voxelConstants.h"

maths::Vec3<float> playerWorldInteraction::getBlockPosInFrontOfPlayer(world::World& world, player::Player& player)
{
	auto t = maths::positionFromRotation<float>(player.rotation) * player.playerReach;

	maths::Vec3<float> scaledPlayerPos = player.position * (float)getVoxelSubdivision();
	maths::Vec3<float> finalBlockPos;
	for (float i = 0.f; i < 100.f * player.playerReach; i++)
	{
		finalBlockPos = maths::Vec3<float>(t.x, t.y, t.z) * i / 100.f / player.playerReach * (float)getVoxelSubdivision();
		auto blockID = world.getBlockID(finalBlockPos + scaledPlayerPos);
		if (blockID != blox::air)break;
	}

	return finalBlockPos + scaledPlayerPos;
}

void playerWorldInteraction::breakBlockInFrontOfPlayer(world::World& world, player::Player& player)
{
	maths::Vec3<float> blockPosInFrontOfPlayer = playerWorldInteraction::getBlockPosInFrontOfPlayer(world, player);
	//std::cout << "Block pos: " << blockPosInFrontOfPlayer.floor(1.f) << std::endl;
	world.setBlockID(blockPosInFrontOfPlayer, blox::air);
}

void playerWorldInteraction::setBlockInFrontOfPlayer(world::World& world, player::Player& player, blox::ID block)
{
	auto oldBlockPos = getBlockPosInFrontOfPlayer(world, player);
	if (world.getBlockID(oldBlockPos) == blox::air) return;
	auto newBlockPos = oldBlockPos + maths::positionFromRotation<float>(player.rotation) * -.5f;
	world.setBlockID(newBlockPos, block);
	if (player.isColliding(world)) world.setBlockID(newBlockPos, blox::air);
}
