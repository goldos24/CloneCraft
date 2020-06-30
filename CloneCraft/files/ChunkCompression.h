#pragma once

#include <fstream>
#include "../world/chunks.h"

namespace chunkCompression
{
	void saveBlocks(chunks::Chunk& chunk, std::ofstream& out);

	void loadBlocks(chunks::Chunk& chunk, std::ifstream& in);
}