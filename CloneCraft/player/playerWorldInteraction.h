#pragma once

#include <cmath>
#include "../player/player.h"
#include "../maths/maths.h"
#include "../world/blocks.h"
#include "../world/chunks.h"

namespace playerWorldInteraction
{

	maths::Vec3<float> getBlockPosInFrontOfPlayer(world::World& world, player::Player& player);

	void breakBlockInFrontOfPlayer(world::World& world, player::Player& player);

	void setBlockInFrontOfPlayer(world::World& world, player::Player& player, blox::ID block);
}