#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "dungeon.h"
#include "seed.h"
#include "file_writer.h"

int max_feeling_count = 10;

int test_seed(int64_t seed)
{
    int feeling_count = 0;
    init(seed);

    for (depth = 1; depth < 25; depth++)
    {
        int f = new_feeling();
        if (f == 3) // 3 = Enemies moving in the darkness
            feeling_count++;

        // Finish early if the number of floors remaining is not enough to match previous best
        // This noticably speeds up the search
        if ((24 - depth) - ((24 - depth) / 5) < max_feeling_count - feeling_count)
            return feeling_count;
    }
    return feeling_count;
}

int64_t get_time()
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return ((int64_t)spec.tv_sec * 1000) + spec.tv_nsec / 1000000;
}

void scan_seeds(int64_t starting_seed, int64_t seeds_to_scan)
{

    printf("\n==============================================================\n");
    printf("[New] Dark floor finder\n");
    printf("==============================================================\n");

    int64_t seed = starting_seed;
    int64_t start_time = get_time();

    for (int64_t seeds_checked = 1; seeds_checked <= seeds_to_scan; seeds_checked++)
    {
        int feeling_count = test_seed(seed);
        if (feeling_count == max_feeling_count)
        {
            printf("Matched previous best (%d): %s\n", feeling_count, display_seed(seed));
            write_seed_to_file(displayed_seed, feeling_count);
        }
        else if (feeling_count > max_feeling_count)
        {
            max_feeling_count = feeling_count;
            printf("===== New best: %d Seed: %s\n", feeling_count, display_seed(seed));
            write_seed_to_file(displayed_seed, feeling_count);
        }
        seed++;

        if (seeds_checked % 200000000 == 0)
        {
            int64_t now = get_time();
            int64_t runtime = now - start_time;
            printf("Checked %lld seeds (%.2f%%) in %.2f seconds (%.2fM seed/min)\n",
                   seeds_checked,
                   seeds_checked * 100.0f / seeds_to_scan,
                   runtime / 1000.0f,
                   seeds_checked / 1000000.0f / (runtime / 60000.0f));
        }
    }
}

int main(int argc, char *argv[])
{
    int64_t starting_seed = 0, seeds_to_scan = 5429503678976LL;
    if (argc >= 2)
        starting_seed = parse_seed(argv[1]);
    printf("%lld", starting_seed);
    if (argc >= 3)
        seeds_to_scan = atoll(argv[2]);
    if (argc >= 4)
        open_file(argv[3]);
    scan_seeds(starting_seed, seeds_to_scan);
}