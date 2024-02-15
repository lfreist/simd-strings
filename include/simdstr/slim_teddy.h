/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#ifndef SIMD_STRING_SLIM_TEDDY_H
#define SIMD_STRING_SLIM_TEDDY_H

#include <stdint.h>

struct Bucket {
        uint8_t pattern_ids[8];
        uint8_t size;
};

#endif//SIMD_STRING_SLIM_TEDDY_H
