#include "blocks.h"



void blox::eliminateFalseID(ID& id)
{
	if (id >= enumSize)  id = air;
}

bool blox::isTransparent(ID blockId)
{
	return blockId == air;
}