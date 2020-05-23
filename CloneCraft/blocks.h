#pragma once
#define func auto
#include<string>

namespace blox 
{
	enum ID : unsigned char
	{
		air,
		stone,
		grass
	};

	func isTransparent(ID blockId) -> bool
	{
		return blockId == air;
	}

	struct Block
	{
		Block() {}
		Block(ID id, std::string name)
		{
			this->id = id;
			this->name = name;
		}

		ID id;
		std::string name;
	};

	namespace 
	{
		func InitBlocks() -> Block*
		{
			auto* blocks = new Block[256];
			blocks[air] = Block(air, "air");
			blocks[stone] = Block(stone, "stone");
			return blocks;
		}

		const Block* blocks = InitBlocks();
	}

	func getByID(ID id) -> Block
	{
		return blocks[id];
	}
}