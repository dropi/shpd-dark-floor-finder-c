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

void write_seed_to_file(char *seed, int feeling_count)
{
    if (log_file == NULL)
        return;
    fprintf(log_file, "%s - %d\n", seed, feeling_count);
}