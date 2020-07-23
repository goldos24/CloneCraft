#include "files.h"
#include "ChunkCompression.h"


namespace saveData
{

	int loadInt(std::ifstream& inputFile)
	{
		saveData::savedInt result;

		for (int j = 0; j < 4; ++j)
			inputFile.get(result.bytes[j]);
		return result.num;
	}

	std::shared_ptr<chunks::Chunk> loadSimpleChunk(std::ifstream& inputFile)
	{
		auto chunk = loadDefaultChunkData(inputFile);
		for (int i = 0; i < maths::cubeof(chunks::size); i++)
		{
			char c;inputFile.get(c);
			chunk->blocks[i] = (blox::ID) c;
		}
		return chunk;
	}

	void saveInt(std::ofstream& outputFile, int data)
	{
		saveData::savedInt i;
		i.num = data;
		for (int j = 0; j < 4; ++j)
			outputFile << i.bytes[j];
	}

	void saveSimpleChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
	{
		outputFile << char(Format::simpleCharFormat);
		saveDefaultChunkData(outputFile, chunk);
		for (int i = 0; i < maths::cubeof(chunks::size); i++)
		{
			outputFile << char(chunk->blocks[i]);
		}
	}

	void Manager::addChunk(std::shared_ptr<chunks::Chunk> chunk)
	{
		chunks.push_back(chunk);
	}

	bool Manager::loadAll(std::string fileName)
	{
		std::ifstream inputFile;
		inputFile.open(fileName + ".save");

		if (!inputFile.is_open()) return false;
		this->seed = loadInt(inputFile);

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

	void Manager::saveAll(std::string worldName)
	{
		std::ofstream outputFile;
		outputFile.open(worldName + ".save");
		saveInt(outputFile, this->seed);
		for (auto chonk : this->chunks)
			if (chonk->chunkPos.x > 0 && chonk->chunkPos.y > 0 && chonk->chunkPos.z > 0) saveCompressedChunk(outputFile, chonk);
			else saveSimpleChunk(outputFile, chonk);
		outputFile.close();
	}

	void Manager::unloadAll()
	{
		this->chunks.clear();
	}

	std::shared_ptr<chunks::Chunk> loadCompressedChunk(std::ifstream& inputFile)
	{
		auto chunk = loadDefaultChunkData(inputFile);

		chunkCompression::loadBlocks(*chunk, inputFile);

		return chunk;
	}

	void saveCompressedChunk(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
	{
		outputFile << char(Format::compressedFormat);
		saveDefaultChunkData(outputFile, chunk);

		chunkCompression::saveBlocks(*chunk, outputFile);
	}

	std::string loadEntityDataString(std::ifstream& inputFile) 
	{
		char c = '\0';
		if (!inputFile.good()) return "";

		inputFile.get(c);

		if (c != '{') std::cout << "Error: invalid symbol for chunk entity data" << std::endl;

		std::string result;
		do
		{
			inputFile.get(c);
			if (c == '}') break;
			result += c;
		}
		while (c != '}');

		return result;
	}

	void putEntityDataString(std::shared_ptr<chunks::Chunk> chunk, std::ofstream& outputFile) 
	{
		outputFile << '{';
		for (auto entity : chunk->entities)
		{
			outputFile << entity->encode();
		}
		outputFile << '}';
	}


	std::shared_ptr<chunks::Chunk> loadDefaultChunkData(std::ifstream& inputFile)
	{

		auto chunk = std::make_shared<chunks::Chunk>();
		chunk->chunkPos.x = loadInt(inputFile);
		chunk->chunkPos.y = loadInt(inputFile);
		chunk->chunkPos.z = loadInt(inputFile);
		auto entityData = loadEntityDataString(inputFile);
		/*if (!loadDataPrinted)
		{
			loadDataPrinted = true;
			std::cout << "Entity Data: (" << entityData << ")\n";
		}*/
		Entity::parseEntities(entityData, chunk->entities);

		return chunk;
	}

	void saveDefaultChunkData(std::ofstream& outputFile, std::shared_ptr<chunks::Chunk> chunk)
	{
		saveInt(outputFile, chunk->chunkPos.x);
		saveInt(outputFile, chunk->chunkPos.y);
		saveInt(outputFile, chunk->chunkPos.z);

		putEntityDataString(chunk, outputFile);
	}
}

