#pragma once
#include <iostream>
extern bool LostPercentChance(double lost);

struct NackInfo
{
    NackInfo() = default;
    explicit NackInfo(long int createdAtMs, uint16_t seq)
        : createdAtMs(createdAtMs), seq(seq)
    {
    }

    long int createdAtMs{ 0u };
    uint16_t seq{ 0u };
    long int sentAtMs{ 0u };
    uint8_t retries{ 0u };
};
