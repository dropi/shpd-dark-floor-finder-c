#pragma once

#include <stdio.h>

FILE *log_file = NULL;

void open_file(char *file_name)
{
    log_file = fopen(file_name, "w");
    if (log_file != NULL)
    {
        setvbuf(log_file, NULL, _IOLBF, BUFSIZ);
    }
}

void write_seed_to_file(char *seed, int *feeling_counts)
{
    if (log_file == NULL)
        return;
    fprintf(log_file, "%s - %dC %dW %dG %dD %dL %dT %dS\n", seed, feeling_counts[0], feeling_counts[1], feeling_counts[2], feeling_counts[3], feeling_counts[4], feeling_counts[5], feeling_counts[6]);
}