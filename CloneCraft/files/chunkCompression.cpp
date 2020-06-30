#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../maths/maths.h"
#include "../world/chunkData.h"
#include "ChunkCompression.h"

struct compressedBitOfText
{
    int len;
    char c;

    std::string toString()
    {
        return "( len: " + std::to_string(this->len) + " char: '" + this->c + "' )";
    }
};

union savedShort
{
    short num;
    char bytes[2];
};

std::vector<compressedBitOfText> compress(char* text)
{
    std::vector<compressedBitOfText> result;
    char current = 0;
    int currentlen = 0;
    for (int i = 0; i < maths::cubeof(chunks::size); ++i)
    {
        if (current == text[i])
        {
            currentlen++;
        }
        else
        {
            result.push_back(compressedBitOfText{ currentlen, current });
            current = text[i];
            currentlen = 1;
        }
    }

    result.push_back(compressedBitOfText{ currentlen, current });

    return result;
}

std::ostream& operator<<(std::ostream& out, std::vector<compressedBitOfText> compressedText)
{
    for (int i = 0; i < compressedText.size(); ++i)
        out << compressedText[i].toString();
    return out;
}

std::string recreate(std::vector<compressedBitOfText> compressedText)
{
    std::string result;

    for (auto compressedBit : compressedText)
    {
        for (int i = 0; i < compressedBit.len; ++i)
        {
            result += compressedBit.c;
        }
    }

    return result;
}

void save(std::vector<compressedBitOfText> compressedText, std::ofstream& out)
{
    for (auto bit : compressedText)
    {
        savedShort s;
        s.num = bit.len;
        out << s.bytes[0] << s.bytes[1] << bit.c;
    }
    savedShort s;
    s.num = -69;
    out << s.bytes[0] << s.bytes[1];
}

void load(std::ifstream& in, std::vector<compressedBitOfText>& result)
{
    while (in.good())
    {
        savedShort s;
        in.get(s.bytes[0]);
        in.get(s.bytes[1]);
        if (s.num == -69) break;
        compressedBitOfText bit;
        bit.len = s.num;
        in.get(bit.c);
        result.push_back(bit);
    }
}

void chunkCompression::saveBlocks(chunks::Chunk& chunk, std::ofstream& out)
{
    auto compressedBlocks = compress((char *) chunk.blocks);
    save(compressedBlocks, out);
}

void chunkCompression::loadBlocks(chunks::Chunk& chunk, std::ifstream& in)
{
    std::vector<compressedBitOfText> compressedBlocks; load(in, compressedBlocks);
    std::string blockString = recreate(compressedBlocks);

    for (int i = 0; i < blockString.size() && i < maths::cubeof(chunks::size); ++i)
    {
        chunk.blocks[i] = (blox::ID) blockString[i];
    }
}