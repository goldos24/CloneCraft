#include "stringEncoding.h"

uint64_t StringToInt(std::string str)
{
    union
    {
        char chars[8];
        uint64_t num;
    } result;
    result.num = 0;
    for (int i = 0; i < 8 && i < str.size(); ++i)
    {
        result.chars[i] = str[i];
    }
    return result.num;
}

std::string IntToString(uint64_t i)
{
    std::string result;
    union
    {
        char chars[8];
        uint64_t num;
    } data;
    data.num = i;
    for (int i = 0; i < 8; ++i)
        result += data.chars[i];
    return result;
}

