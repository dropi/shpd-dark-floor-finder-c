#pragma once

#include <stdio.h>
#include <stdint.h>

int64_t level_seed, dungeon_seed;

const uint64_t multiplier = 0x5deece66dLL;
const uint64_t mask = (1LL << 48) - 1;
const uint64_t addend = 0xBLL;

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

int32_t next(int bits, int64_t *seed)
{
    *seed = (*seed * multiplier + addend) & mask;
    return (int32_t)(*seed >> (48 - bits));
}

int64_t next_long(int64_t *seed)
{
    return ((int64_t)(next(32, seed)) << 32) + next(32, seed);
}

int32_t next_int(int32_t bound)
{
    int32_t r = next(31, &level_seed);
    int32_t m = bound - 1;
    if ((bound & m) == 0) // i.e., bound is a power of 2
        r = (int32_t)((bound * (int64_t)r) >> 31);
    else
    { // reject over-represented candidates
        for (int32_t u = r;
             u - (r = u % bound) + m < 0;
             u = next(31, &level_seed))
            ;
    }
    return r;
}

void set_dungeon_seed(int64_t s)
{
    dungeon_seed = scramble_seed(s);
}

void set_next_level_seed()
{
    level_seed = scramble_seed(next_long(&dungeon_seed));
}