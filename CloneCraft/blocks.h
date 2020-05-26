#pragma once
#include <string>
#include "textures.h"

namespace blox 
{
	enum ID : unsigned char
	{
		air,
		stone,
		grass,

		enumSize
	};

	void eliminateFalseID(ID& id)
	{
		if (id >= enumSize)  id = air; 
	}

	auto isTransparent(ID blockId) -> bool
	{
		return blockId == air;
	}

	struct Block
	{
		Block() {}
		Block(ID id, std::string name, textures::BlockTexture* texture)
		{
			this->id = id;
			this->name = name;
			this->texture = texture;
		}

		textures::BlockTexture* texture;
		ID id;
		std::string name;
	};

	namespace 
	{
		Block* InitBlocks()
		{
			auto* blocks = new Block[256];
			blocks[air] = Block(air, "air", nullptr); //TODO nullptr
			blocks[stone] = Block(stone, "stone", textures::blockTextures::stone);

			blocks[grass] = Block(grass, "grass", textures::blockTextures::grass);
			return blocks;
		}

		Block* blocks = InitBlocks();
	}

	Block& getByID(ID id)
	{
		return blocks[id];
	}
}
