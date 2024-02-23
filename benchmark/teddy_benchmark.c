/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include <stdio.h>
#include <string.h>

#include "statistics/statistics.h"
#include "timer/timer.h"

#include <simdstr/fat_teddy.h>
#include <simdstr/slim_teddy.h>

void Benchmark_FatTeddy() {
        __declspec(align(16)) char haystack[] = "sdfj kjdf foo! anyways... this is how it works, so it is okay.";
        char* patterns[] = {"foo", "bar", "bat"};

        Pattern pats[3];
        for (uint8_t i = 0; i < 3; ++i)
        {
                pats[i].begin = patterns[i];
                pats[i].size = strlen(patterns[i]);
        }

        FatTeddy teddy;
        FatTeddy_init(&teddy, pats, 3);

        Match match = FatTeddy_find(&teddy, haystack, strlen (haystack));
        if (match.pattern_id >= 0) {
                printf ("Match found at position %li for pattern %i\n", (long)(match.begin - &haystack[0]), match.pattern_id);
        } else {
                printf("No pattern found.\n");
        }
}

void Benchmark_SlimTeddy() {
        __declspec(align(16)) char haystack[] = "sdfj kjdf foo! anyways... this is how it works, so it is okay.";
        char* patterns[] = {"foo", "bar", "bat"};

        Pattern pats[3];
        for (uint8_t i = 0; i < 3; ++i)
        {
                pats[i].begin = patterns[i];
                pats[i].size = strlen(patterns[i]);
        }

        SlimTeddy teddy;
        SlimTeddy_init(&teddy, pats, 3, 1);

        Match match = SlimTeddy_find(&teddy, haystack, strlen (haystack));
        if (match.pattern_id >= 0) {
                printf ("Match found at position %li for pattern %i\n", (long)(match.begin - &haystack[0]), match.pattern_id);
        } else {
                printf("No pattern found.\n");
        }
}


int main() {
        Benchmark_FatTeddy();
        Benchmark_SlimTeddy();

        return 0;
}