#pragma once

namespace blox 
{
	enum ID : unsigned char
	{
		air,
		stone,
		grass,
		dirt,
		wewd,
		leaves,

		enumSize
	};

	void eliminateFalseID(ID& id);

	bool isTransparent(ID blockId);
}
