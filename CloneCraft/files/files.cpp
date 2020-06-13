#include "files.h"

int saveData::loadInt(std::ifstream& inputFile)
{
	int result;
	inputFile >> result;
	char c; inputFile.get(c); // Ignoring the next char which is '\n'
	return result;
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
	outputFile << '\n' << data << '\n';
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

void saveData::Manager::loadAll()
{
	std::ifstream inputFile;
	inputFile.open("savedChunk");
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
			/*case saveData::Format::compressedFormat:
				this->addChunk(loadCompressedChunk(inputFile));
				break;*/
		default:
			break;
		}
	}
	inputFile.close();
}

void saveData::Manager::saveAll()
{
	std::ofstream outputFile;
	outputFile.open("savedChunk");
	for (auto chonk : this->chunks)
		saveSimpleChunk(outputFile, chonk);
	outputFile.close();
}