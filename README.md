# shpd-dark-floor-finder-c

[shpd-dark-floor-finder](https://github.com/dropi/shpd-dark-floor-finder), remade in C

Note: The project is currently in a "rough MVP" state. If you decide to run in it yourself for some reason, expect issues.

# How to build

GCC flags that worked best for me:

```sh
gcc -Ofast -march=native -flto --static main.c -o seed_finder.exe
```

# How to run

```sh
./seed_finder.exe [STARTING_SEED] [NUMBER_OF_SEEDS_TO_SCAN] [OUTPUT_FILE]
```

For example:

```sh
./seed_finder.exe AAA-AAA-AAA 26103383072 report-A.txt
```
