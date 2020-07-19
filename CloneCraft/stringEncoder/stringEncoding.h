#pragma once

#include<string>
#include<cstdint>

uint64_t StringToInt(std::string str);
std::string IntToString(uint64_t i);

constexpr uint64_t CptrToInt(const char str[9])
{
    uint64_t result = 0;
    for (uint64_t i = 0; i < 8; ++i)
    {
        result += uint64_t(str[i]) << uint64_t(i * 8);
    }
    return result;
}