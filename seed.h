#pragma once

#include <stdint.h>
#include <stdio.h>

char displayed_seed[] = "AAA-AAA-AAA";

char *display_seed(int64_t seed)
{
    for (int i = 10; i >= 0; i--)
    {
        if (i == 3 || i == 7)
            continue;
        char c = 'A' + (seed % 26);
        displayed_seed[i] = c;
        seed /= 26;
    }
    return displayed_seed;
}

int64_t parse_seed(const char *seed_text)
{ // Zero validation
    int64_t result = 0;
    for (int i = 0; i <= 10; i++)
    {
        if (i == 3 || i == 7)
            continue;
        result *= 26;
        result += (seed_text[i] - 'A');
    }
    return result;
}