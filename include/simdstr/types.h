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
        char* begin;
        uint64_t size;
} Pattern;

typedef struct {
        int16_t pattern_id;

        char* begin;
        char* end;
} Match;

extern inline Match Match_empty()
{
        Match match;
        match.pattern_id = -1;
        match.begin = NULL;
        match.end = NULL;
        return match;
}

#endif//SIMD_STRING_TYPES_H
