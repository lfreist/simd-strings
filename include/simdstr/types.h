/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#ifndef SIMD_STRING_TYPES_H
#define SIMD_STRING_TYPES_H

#include <stdint.h>

typedef struct {
        char* start;
        uint64_t size;
} Pattern;

typedef struct {
        uint16_t pattern_id;

        char* start;
        char* end;
} Match;

extern inline Match Match_empty()
{
        Match match;
        match.pattern_id = -1;
        match.start = NULL;
        match.end = NULL;
        return match;
}

#endif//SIMD_STRING_TYPES_H
