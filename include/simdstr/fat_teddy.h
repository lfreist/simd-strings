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

struct Bucket {
        uint8_t pattern_ids[16];
        uint8_t size;
};

struct PatternMask {
        uint8_t lo[32];
        uint8_t hi[32];

        __m256i v_lo;
        __m256i v_hi;
};

void pattern_mask_init(struct PatternMask* pattern_mask, struct Bucket* buckets, char** patterns);

void pattern_mask_add_fat(struct PatternMask* mask, char byte, uint8_t bucket_id);

void pattern_mask_finish(struct PatternMask* mask);

struct FatTeddy {
        struct PatternMask pattern_mask;

        char** patterns;
        uint8_t num_patterns;

        struct Bucket buckets[16];
};

void fat_teddy_init(struct FatTeddy* teddy, char** patterns, uint8_t num_patterns);

struct Match fat_teddy_find(struct FatTeddy* teddy, char* str, size_t str_size);

struct Match fat_teddy_find_next(struct FatTeddy* teddy, char* str, size_t str_size);

struct Match fat_teddy_verify(struct FatTeddy* teddy, __m256i* candidate, char* str, size_t str_size);

struct Match fat_teddy_verify64(struct FatTeddy* teddy, uint64_t lane, char* str, size_t str_size);

__m256i lookup_1(__m256i* chunk, struct PatternMask* pattern_mask);

struct Match {
        int pattern_id;
        char* begin;
        char* end;
};

struct Match match_empty();

#endif//SIMD_STRING_FAT_TEDDY_H
