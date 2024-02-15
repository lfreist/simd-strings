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

int main() {
        __declspec(align(16)) char haystack[] = "sdfj kjdfg k foo! anyways... this is how it works, so it is okay.";
        char* patterns[] = {"foo", "bar", "bat"};

        struct FatTeddy teddy;
        fat_teddy_init(&teddy, patterns, 3);

        struct Match match = fat_teddy_find(&teddy, haystack, strlen (haystack));
        if (match.pattern_id >= 0) {
                printf ("Match found at position %li for pattern %s\n", (long)(match.begin - &haystack[0]), teddy.patterns[match.pattern_id]);
        } else {
                printf("No pattern found.\n");
        }

        return 0;
}