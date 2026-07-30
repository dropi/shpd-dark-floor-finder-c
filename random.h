#pragma once

#include <stdio.h>
#include <stdint.h>

int64_t level_seed, dungeon_seed;

const uint64_t multiplier = 0x5deece66dLL;
const uint64_t mask = (1LL << 48) - 1;
const uint64_t addend = 0xBL;

uint64_t scramble_seed(uint64_t s)
{
    s ^= s >> 32;
    s *= 0xbea225f9eb34556dLL;
    s ^= s >> 29;
    s *= 0xbea225f9eb34556dLL;
    s ^= s >> 32;
    s *= 0xbea225f9eb34556dLL;
    s ^= s >> 29;
    return (s ^ multiplier) & mask;
}

void set_dungeon_seed(int64_t s)
{
    dungeon_seed = scramble_seed(s);
}

void set_level_seed(int64_t s)
{
    level_seed = scramble_seed(s);
}

int32_t next_global(int bits)
{
    dungeon_seed = (dungeon_seed * multiplier + addend) & mask;
    return (int32_t)(dungeon_seed >> (48 - bits));
}

int32_t next(int bits)
{
    level_seed = (level_seed * multiplier + addend) & mask;
    return (int32_t)(level_seed >> (48 - bits));
}

int64_t next_long_global()
{
    return ((int64_t)(next_global(32)) << 32) + next_global(32);
}

void set_next_level_seed()
{
    set_level_seed(next_long_global());
}

int64_t next_long()
{
    return ((int64_t)(next(32)) << 32) + next(32);
}

int32_t next_int(int32_t bound)
{
    int32_t r = next(31);
    int32_t m = bound - 1;
    if ((bound & m) == 0) // i.e., bound is a power of 2
        r = (int32_t)((bound * (int64_t)r) >> 31);
    else
    { // reject over-represented candidates
        for (int32_t u = r;
             u - (r = u % bound) + m < 0;
             u = next(31))
            ;
    }
    return r;
}