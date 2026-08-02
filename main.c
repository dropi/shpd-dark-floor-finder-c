#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "dungeon.h"
#include "seed.h"
#include "file_writer.h"

static volatile int interrupted = 0;

int max_feeling_count = 13;
int feeling_counts[7];

void test_seed(int64_t seed)
{
    feeling_counts[0] = 0;
    feeling_counts[1] = 0;
    feeling_counts[2] = 0;
    feeling_counts[3] = 0;
    feeling_counts[4] = 0;
    feeling_counts[5] = 0;
    feeling_counts[6] = 0;

    init(seed);

    for (depth = 1; depth < 25; depth++)
    {
        int f = new_feeling();
        if (f <= 6)
        {
            feeling_counts[f]++;
        }

        // Finish early if the number of floors remaining is not enough to match previous best
        // This noticably speeds up the search
        int threshold = max_feeling_count - (24 - depth) + ((24 - depth) / 5);
        if (feeling_counts[0] < threshold &&
            feeling_counts[1] < threshold &&
            feeling_counts[2] < threshold &&
            feeling_counts[3] < threshold &&
            feeling_counts[4] < threshold &&
            feeling_counts[5] < threshold &&
            feeling_counts[6] < threshold)
            return;
    }
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
        test_seed(seed);
        if (feeling_counts[0] >= max_feeling_count ||
            feeling_counts[1] >= max_feeling_count ||
            feeling_counts[2] >= max_feeling_count ||
            feeling_counts[3] >= max_feeling_count ||
            feeling_counts[4] >= max_feeling_count ||
            feeling_counts[5] >= max_feeling_count ||
            feeling_counts[6] >= max_feeling_count)
        {
            printf("%s - %dC %dW %dG %dD %dL %dT %dS\n", display_seed(seed), feeling_counts[0], feeling_counts[1], feeling_counts[2], feeling_counts[3], feeling_counts[4], feeling_counts[5], feeling_counts[6]);
            write_seed_to_file(displayed_seed, feeling_counts);
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

        if (seeds_checked % 1000000 == 0 && interrupted) // If we check the flag every once in a while it might be faster, idk
        {
            printf("Run interrupted. Continue from: %s %lld\n", display_seed(seed), seeds_to_scan - seeds_checked);
            write_interrupt_to_file(displayed_seed, seeds_to_scan - seeds_checked);
            break;
        }
    }
}

void interruptHandler(int something)
{
    interrupted = 1;
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

    signal(SIGINT, interruptHandler);
    scan_seeds(starting_seed, seeds_to_scan);
}