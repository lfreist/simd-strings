/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#ifndef SIMD_STRING_SLIM_TEDDY_H
#define SIMD_STRING_SLIM_TEDDY_H

#include <immintrin.h>
#include <stdint.h>

#include <simdstr/types.h>

// --- SLimBucket -----------------------------------------------------------------------------------------------------
/**
 * SlimBucket
 */
typedef struct {
        uint8_t size;
        uint8_t pattern_ids[8];
} SlimBucket;
// ___ SlimBucket _____________________________________________________________________________________________________

// --- SLimPatternMask ------------------------------------------------------------------------------------------------
/**
 * SLimPatternMask
 */
typedef struct {
        uint8_t lo[32];
        uint8_t hi[32];

        __m128i v_lo;
        __m128i v_hi;
} SlimPatternMask;

void SlimPatternMask_init (SlimPatternMask* self, SlimBucket* bucket, Pattern* patterns);

void SlimPatternMask_add (SlimPatternMask* self, char byte, uint8_t bucket_id);

void SlimPatternMask_build (SlimPatternMask* self);
// ___ SlimPatternMask ________________________________________________________________________________________________

// --- SlimTeddy ------------------------------------------------------------------------------------------------------
/**
 * SlimTeddy
 */
typedef struct {
        SlimPatternMask pattern_mask;
        SlimBucket buckets[8];

        Pattern* patterns;
        uint8_t num_patterns;
} SlimTeddy;

void SlimTeddy_init (SlimTeddy* self, Pattern* patterns, uint8_t num_patterns);

Match SlimTeddy_find (SlimTeddy* self, char* str, size_t str_size);

Match SlimTeddy_find_one (SlimTeddy* self, char* cur, size_t cur_size);

Match SlimTeddy_verify (SlimTeddy* self, __m128i* candidate, char* cur, size_t cur_size);

Match SlimTeddy_verify64 (SlimTeddy* self, uint64_t lane, char* cur, size_t cur_size);

__m128i lookup_1 (__m128i* chunk, SlimPatternMask* mask);

// ___ SlimTeddy ______________________________________________________________________________________________________

#endif//SIMD_STRING_SLIM_TEDDY_H
