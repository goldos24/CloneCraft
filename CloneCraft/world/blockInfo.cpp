#include "blockInfo.h"

blox::Block::Block() {}

blox::Block::Block(ID id, std::string name, textures::BlockTexture* texture)
{
	this->id = id;
	this->name = name;
	this->texture = texture;
}

blox::Block* blox::InitBlocks()
{
	auto* blocks = new Block[256];
	blocks[air] = Block(air, "air", nullptr); //TODO nullptr

	blocks[stone] = Block(stone, "stone", textures::blockTextures::stone);
	blocks[grass] = Block(grass, "grass", textures::blockTextures::grass);
	blocks[dirt] = Block(dirt, "dirt", textures::blockTextures::dirt);
	blocks[wewd] = Block(wewd, "wood", textures::blockTextures::wewd);
	blocks[leaves] = Block(leaves, "leaves", textures::blockTextures::leaves);

	return blocks;
}

blox::Block& blox::getByID(ID id)
{
	return blocks[id];
}

blox::Block& blox::getByName(std::string name)
{
	for (int i = 0; i < enumSize; i++)
	{
		if (blocks[i].name == name)
		{
			return blocks[i];
		}
	}
	std::cout << "Couldn't find block with ID: " << name << std::endl;
	return blocks[air];
}

std::ostream& operator << (std::ostream& output, blox::ID id)
{
	return output << blox::getByID(id).name;
}