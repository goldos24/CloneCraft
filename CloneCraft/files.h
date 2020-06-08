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

	union SavedInt
	{
		SavedInt() {}

		SavedInt(int num)
		{
			this->num = num;
		}

		int64_t num;

		char bytes[sizeof(int64_t)];
	};

	enum class Format : unsigned char
	{
		simpleCharFormat = 1
	};

	SavedInt loadInt(std::ifstream& inputFile)
	{
		SavedInt result;
		for (int i = 0; i < sizeof(int64_t); i++)
		{
			char c;inputFile.get(c);
			result.bytes[i] = c;
		}
		return result;
	}

	std::shared_ptr<chunks::Chunk> loadSimpleChunk(std::ifstream& inputFile)
	{
		auto chunk = std::make_shared<chunks::Chunk>();
		chunk->chunkPos.x = (int)loadInt(inputFile).num; if (chunk->chunkPos.x == 1)chunk->chunkPos.x = 0;
		chunk->chunkPos.y = (int)loadInt(inputFile).num; if (chunk->chunkPos.y == 1)chunk->chunkPos.y = 0;
		chunk->chunkPos.z = (int)loadInt(inputFile).num; if (chunk->chunkPos.z == 1)chunk->chunkPos.z = 0;
		for (int i = 0; i < maths::cubeof(chunks::size); i++)
		{
			char c;inputFile.get(c);
			chunk->blocks[i] = (blox::ID) c;
		}
		return chunk;
	}

	void saveInt(std::ofstream& outputFile, int data)
	{
		SavedInt encodedData(data);
		for (int i = 0; i < sizeof(int64_t); i++)
		{
			outputFile << encodedData.bytes[i];
		}
	}

	void saveSimpleChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
	{
		outputFile << char(Format::simpleCharFormat);
		if (chunk->chunkPos == maths::Vec3i{ 0, 0, 0 })
		{
			for (int i = 0; i < 3; i++)
				saveInt(outputFile, 1);
		}
		else
		{
			saveInt(outputFile, chunk->chunkPos.x);
			saveInt(outputFile, chunk->chunkPos.y);
			saveInt(outputFile, chunk->chunkPos.z);
		}
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