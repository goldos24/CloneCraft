#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cstdint>
#include "maths.h"
#include "chunkData.h"
#include "chunks.h"
#include "blocks.h"

namespace saveData 
{

	enum class Format : unsigned char
	{
		simpleCharFormat = 1
	};

	int loadInt(std::ifstream& inputFile)
	{
		int result;
		inputFile >> result;
		char c; inputFile.get(c); // Ignoring the next char which is '\n'
		return result;
	}

	std::shared_ptr<chunks::Chunk> loadSimpleChunk(std::ifstream& inputFile)
	{
		auto chunk = std::make_shared<chunks::Chunk>();
		chunk->chunkPos.x = loadInt(inputFile);
		chunk->chunkPos.y = loadInt(inputFile);
		chunk->chunkPos.z = loadInt(inputFile);
		bool pront = false;
		if (chunk->chunkPos.x == 0 && chunk->chunkPos.y == 0 && chunk->chunkPos.z == 0) pront = true;
		for (int i = 0; i < maths::cubeof(chunks::size); i++)
		{
			char c;inputFile.get(c);
			chunk->blocks[i] = (blox::ID) c;
		}
		return chunk;
	}

	void saveInt(std::ofstream& outputFile, int data)
	{
		outputFile << '\n' << data << '\n';
	}

	void saveSimpleChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
	{
		outputFile << char(Format::simpleCharFormat);
			saveInt(outputFile, chunk->chunkPos.x);
			saveInt(outputFile, chunk->chunkPos.y);
			saveInt(outputFile, chunk->chunkPos.z);
		for (int i = 0; i < maths::cubeof(chunks::size); i++)
		{
			outputFile << char(chunk->blocks[i]);
		}
	}

	struct Manager
	{
		std::vector<std::shared_ptr<chunks::Chunk>> chunks;

		void addChunk(std::shared_ptr<chunks::Chunk> chunk)
		{
			chunks.push_back(chunk);
		}

		void loadAll()
		{
			std::ifstream inputFile;
			inputFile.open("savedChunk");
			while (inputFile.good())
			{
				char chunkFormatC;
				inputFile.get(chunkFormatC);
				Format chunkFormat = (Format) chunkFormatC;
				switch (chunkFormat)
				{
				case saveData::Format::simpleCharFormat:
					this->addChunk(loadSimpleChunk(inputFile));
					break;
				default:
					break;
				}
			}
			inputFile.close();
		}

		void saveAll() 
		{
			std::ofstream outputFile;
			outputFile.open("savedChunk");
			for (auto chonk : this->chunks)
				saveSimpleChunk(outputFile, chonk);
			outputFile.close();
		}
	};

}