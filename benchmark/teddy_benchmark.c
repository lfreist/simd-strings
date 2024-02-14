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

#include <simdstr/teddy.h>

int main() {
        char haystack[] __attribute__((aligned(16))) = "text with only one pattern foo! anyways... this is how it works, so it is okay.";
        char* patterns[] = {"foo", "bar", "bat"};

        struct FatTeddy teddy;
        fat_teddy_init(&teddy, patterns, 3);

        struct Match match = teddy_find(&teddy, haystack, strlen (haystack));
        if (match.pattern_id >= 0) {
                printf ("Match found at position %li for patter %s\n", match.begin - &haystack[0], teddy.patterns[match.pattern_id]);
        } else {
                printf("No pattern found.\n");
        }

        return 0;
}