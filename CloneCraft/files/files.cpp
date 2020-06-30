#include "files.h"
#include "ChunkCompression.h"

int saveData::loadInt(std::ifstream& inputFile)
{
	saveData::savedInt result;

	for (int j = 0; j < 4; ++j)
		inputFile.get(result.bytes[j]); 
	return result.num;
}

std::shared_ptr<chunks::Chunk> saveData::loadSimpleChunk(std::ifstream& inputFile)
{
	auto chunk = std::make_shared<chunks::Chunk>();
	chunk->chunkPos.x = loadInt(inputFile);
	chunk->chunkPos.y = loadInt(inputFile);
	chunk->chunkPos.z = loadInt(inputFile);
	for (int i = 0; i < maths::cubeof(chunks::size); i++)
	{
		char c;inputFile.get(c);
		chunk->blocks[i] = (blox::ID) c;
	}
	return chunk;
}

void saveData::saveInt(std::ofstream& outputFile, int data)
{
	saveData::savedInt i;
	i.num = data;
	for (int j = 0; j < 4; ++j)
		outputFile << i.bytes[j];
}

void saveData::saveSimpleChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
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

void saveData::Manager::addChunk(std::shared_ptr<chunks::Chunk> chunk)
{
	chunks.push_back(chunk);
}

bool saveData::Manager::loadAll(std::string fileName)
{
	std::ifstream inputFile;
	inputFile.open(fileName + ".save");
	
	if (!inputFile.is_open()) return false;

	while (inputFile.good())
	{
		char chunkFormatC;
		inputFile.get(chunkFormatC);
		Format chunkFormat = (Format)chunkFormatC;
		switch (chunkFormat)
		{
		case saveData::Format::simpleCharFormat:
			this->addChunk(loadSimpleChunk(inputFile));
			break;
		case saveData::Format::compressedFormat:
			this->addChunk(loadCompressedChunk(inputFile));
			break;
		default:
			break;
		}
	}
	
	inputFile.close();
	return true;
}

void saveData::Manager::saveAll(std::string worldName)
{
	std::ofstream outputFile;
	outputFile.open(worldName + ".save");
	for (auto chonk : this->chunks)
		if (chonk->chunkPos.x > 0 && chonk->chunkPos.y > 0 && chonk->chunkPos.z > 0) saveCompressedChunk(outputFile, chonk);
		else saveSimpleChunk(outputFile, chonk);
	outputFile.close();
}

void saveData::Manager::unloadAll()
{
	this->chunks.clear();
}

std::shared_ptr<chunks::Chunk> saveData::loadCompressedChunk(std::ifstream& inputFile)
{
	auto chunk = std::make_shared<chunks::Chunk>();
	chunk->chunkPos.x = loadInt(inputFile);
	chunk->chunkPos.y = loadInt(inputFile);
	chunk->chunkPos.z = loadInt(inputFile);


	chunkCompression::loadBlocks(*chunk, inputFile);

	return chunk;
}

void saveData::saveCompressedChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
{
	outputFile << char(Format::compressedFormat);
	saveInt(outputFile, chunk->chunkPos.x);
	saveInt(outputFile, chunk->chunkPos.y);
	saveInt(outputFile, chunk->chunkPos.z);

	chunkCompression::saveBlocks(*chunk, outputFile);
}