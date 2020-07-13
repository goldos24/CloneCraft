#pragma once
#include "../world/blocks.h"
#include <string>
#include <iostream>
#include "../textures/textures.h"

namespace blox
{

	struct Block
	{
		Block();
		Block(ID id, std::string name, textures::BlockTexture* texture);

		textures::BlockTexture* texture;
		ID id;
		std::string name;
	};

	Block* InitBlocks();

	static Block* blocks = InitBlocks();

	Block& getByID(ID id);

	Block& getByName(std::string name);
}

std::ostream& operator << (std::ostream& output, blox::ID id);