#pragma once

namespace blox 
{
	enum ID : unsigned char
	{
		air,
		stone,
		grass,
		dirt,

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
}
