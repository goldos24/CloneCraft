#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cstdint>
#include "../maths/maths.h"
#include "../world/chunkData.h"
#include "../world/chunks.h"
#include "../world/blocks.h"

namespace saveData 
{

	enum class Format : unsigned char
	{
		simpleCharFormat = 1,
		compressedFormat
	};

	union savedInt
	{
		int num;
		char bytes[4];
	};

	int loadInt(std::ifstream& inputFile);

	std::shared_ptr<chunks::Chunk> loadSimpleChunk(std::ifstream& inputFile);

	std::shared_ptr<chunks::Chunk> loadCompressedChunk(std::ifstream& inputFile); 

	void saveInt(std::ofstream& outputFile, int data);

	void saveSimpleChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk);

	void saveCompressedChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk); 

	struct Manager
	{
		std::vector<std::shared_ptr<chunks::Chunk>> chunks;

		void addChunk(std::shared_ptr<chunks::Chunk> chunk);

		bool loadAll(std::string worldName);

		void saveAll(std::string worldName);

		void unloadAll();
	};

}