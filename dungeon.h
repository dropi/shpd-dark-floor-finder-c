#pragma once

#include "random.h"

int depth;

int strength_potions;
int upgrade_scrolls;
int arcane_styli;
int ench_stone;
int int_stone;
int trinket_cata;

void reset_drops()
{
    strength_potions = 0;
    upgrade_scrolls = 0;
    arcane_styli = 0;
    ench_stone = 0;
    int_stone = 0;
    trinket_cata = 0;
}

void pos_needed()
{
    // 2 POS each floor set
    int posLeftThisSet = 2 - (strength_potions - (depth / 5) * 2);
    if (posLeftThisSet <= 0)
        return;

    int floorThisSet = (depth % 5);

    // pos drops every two floors, (numbers 1-2, and 3-4) with a 50% chance for the
    // earlier one each time.
    int targetPOSLeft = 2 - floorThisSet / 2;
    if (floorThisSet % 2 == 1 && next_int(2) == 0)
        targetPOSLeft--;

    if (targetPOSLeft < posLeftThisSet)
        strength_potions++;
}

void sou_needed()
{
    int souLeftThisSet;
    // 3 SOU each floor set
    souLeftThisSet = 3 - (upgrade_scrolls - (depth / 5) * 3);
    if (souLeftThisSet <= 0)
        return;

    int floorThisSet = (depth % 5);
    // chance is floors left / scrolls left
    if (next_int(5 - floorThisSet) < souLeftThisSet)
        upgrade_scrolls++;
}

void as_needed()
{
    // 1 AS each floor set
    int asLeftThisSet = 1 - (arcane_styli - (depth / 5));
    if (asLeftThisSet <= 0)
        return;

    int floorThisSet = (depth % 5);
    // chance is floors left / scrolls left
    if (next_int(5 - floorThisSet) < asLeftThisSet)
        arcane_styli++;
}

void ench_stone_needed()
{
    // 1 enchantment stone, spawns on chapter 2 or 3
    if (ench_stone == 0)
    {
        int region = 1 + depth / 5;
        if (region > 1)
        {
            int floorsVisited = depth - 5;
            if (floorsVisited > 4)
                floorsVisited--; // skip floor 10
            if (next_int(9 - floorsVisited) == 0)
                ench_stone = 1;
        }
    }
    return;
}

void int_stone_needed()
{
    // one stone on floors 1-3
    if (depth < 5 && int_stone == 0 && next_int(4 - depth) == 0)
        int_stone = 1;
}

void trinket_cata_needed()
{
    // one trinket catalyst on floors 1-3
    if (depth < 5 && trinket_cata == 0 && next_int(4 - depth) == 0)
        trinket_cata = 1;
}

void init(int64_t seed)
{
    reset_drops();
    set_dungeon_seed(seed);
    next_long_global(); // Fire once to skip f0
}

int new_feeling()
{
    int feeling = -1;
    set_next_level_seed();
    if (depth % 5 != 0)
    {
        pos_needed();
        sou_needed();
        as_needed();
        ench_stone_needed();
        int_stone_needed();
        trinket_cata_needed();

        if (depth > 1)
        {
            feeling = next_int(14);
        }
    }
    return feeling;
}