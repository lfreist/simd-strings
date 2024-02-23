/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#ifndef SIMD_STRING_FAT_TEDDY_H
#define SIMD_STRING_FAT_TEDDY_H

#include <immintrin.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <simdstr/utils/utils.h>
#include <simdstr/types.h>

typedef struct {
       uint8_t pattern_ids[16];
       uint8_t size;
} FatBucket;

typedef struct {
       uint8_t lo[32];
       uint8_t hi[32];

       __m256i v_lo;
       __m256i v_hi;
} FatPatternMask;

void pattern_mask_init(FatPatternMask* pattern_mask, FatBucket* buckets, char** patterns);

void pattern_mask_add_fat(FatPatternMask* mask, char byte, uint8_t bucket_id);

void pattern_mask_finish(FatPatternMask* mask);

typedef struct {
       FatPatternMask pattern_mask;

       char** patterns;
       uint8_t num_patterns;

       FatBucket buckets[16];
} FatTeddy;

void fat_teddy_init(FatTeddy* teddy, char** patterns, uint8_t num_patterns);

Match fat_teddy_find(FatTeddy* teddy, char* str, size_t str_size);

Match fat_teddy_find_next(FatTeddy* teddy, char* str, size_t str_size);

Match fat_teddy_verify(FatTeddy* teddy, __m256i* candidate, char* str, size_t str_size);

Match fat_teddy_verify64(FatTeddy* teddy, uint64_t lane, char* str, size_t str_size);

__m256i mm256_lookup_1(__m256i* chunk, FatPatternMask* pattern_mask);

#endif//SIMD_STRING_FAT_TEDDY_H