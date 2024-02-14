/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#ifndef SIMD_STRING_TEDDY_H
#define SIMD_STRING_TEDDY_H

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

void add_pattern_to_mask(struct PatternMask* mask, char byte, uint8_t bucket_id)
{
        uint64_t byte_lo = byte & 0xf;
        uint64_t byte_hi = ((byte >> 4) & 0xf);
        if (bucket_id < 8)
        {
                mask->lo[byte_lo] |= 1 << bucket_id;
                mask->hi[byte_hi] |= 1 << bucket_id;
        } else
        {
                mask->lo[byte_lo + 16] |= 1 << (bucket_id % 8);
                mask->hi[byte_hi + 16] |= 1 << (bucket_id % 8);
        }
}

void pattern_mask_finish(struct PatternMask* mask)
{
        mask->v_lo = _mm256_loadu_si256 ((__m256i*)mask->lo);
        mask->v_hi = _mm256_loadu_si256 ((__m256i*)mask->hi);
}

void pattern_mask_init(struct PatternMask* pattern_mask, struct Bucket* buckets, char** patterns)
{
        memset (pattern_mask->lo, 0, 32);
        memset (pattern_mask->hi, 0, 32);

        for (uint8_t bucket_id = 0; bucket_id < 16; ++bucket_id)
        {
                for (uint8_t i = 0; i < buckets[bucket_id].size; ++i)
                {
                        char* pattern = patterns[buckets[bucket_id].pattern_ids[i]];
                        add_pattern_to_mask (pattern_mask, pattern[0], bucket_id);
                }
        }
}

struct FatTeddy {
        struct PatternMask pattern_mask;

        char** patterns;
        uint8_t num_patterns;

        struct Bucket buckets[16];
};

void fat_teddy_init(struct FatTeddy* teddy, char** patterns, uint8_t num_patterns)
{
        teddy->patterns = patterns;
        teddy->num_patterns = num_patterns;

        // init buckets
        for (int i = 0; i < 16; ++i)
        {
                teddy->buckets[i].size = 0;
        }

        struct Bucket* current_bucket = &teddy->buckets[0];
        uint8_t current_bucket_id = 0;

        for (uint8_t pattern_id = 0; pattern_id < num_patterns; ++pattern_id)
        {
                current_bucket->pattern_ids[current_bucket->size] = pattern_id;
                current_bucket->size++;

                if (current_bucket->size == 16)
                {
                        current_bucket_id++;
                        if (current_bucket_id > 16) {
                                // panic
                                assert (0);
                        }
                        current_bucket = &teddy->buckets[current_bucket_id];
                }
        }

        pattern_mask_init (&teddy->pattern_mask, teddy->buckets, teddy->patterns);
        pattern_mask_finish (&teddy->pattern_mask);
}

struct Match {
        int pattern_id;
        char* begin;
        char* end;
};

struct Match match_empty() {
        struct Match match;
        match.pattern_id = -1;
        match.begin = NULL;
        match.end = NULL;

        return match;
}

__m256i lookup_1(__m256i* chunk, struct PatternMask* pattern_mask)
{
        const __m256i mask = _mm256_set1_epi8 (0xf);
        __m256i chunk_lo = _mm256_and_si256 (*chunk, mask);
        __m256i chunk_hi = _mm256_srli_epi16 (*chunk, 4);
        chunk_hi = _mm256_and_si256 (chunk_hi, mask);

        const __m256i match_lo = _mm256_shuffle_epi8 (pattern_mask->v_lo, chunk_lo);
        const __m256i match_hi = _mm256_shuffle_epi8 (pattern_mask->v_hi, chunk_hi);

        return _mm256_and_si256 (match_lo, match_hi);
}

struct Match teddy_verify64(struct FatTeddy* teddy, uint64_t lane, char* str, size_t str_size)
{
        while (lane != 0)
        {
                int bit = ctz_64 (lane);
                lane ^= (1 << bit);

                str += (int)(bit / 16);
                struct Bucket* bucket = &teddy->buckets[bit % 16];

                for (int i = 0; i < bucket->size; ++i)
                {
                        uint8_t pattern_id = bucket->pattern_ids[i];
                        char* pattern = teddy->patterns[pattern_id];
                        if (memcmp (str, pattern, strlen(pattern)) == 0)
                        {
                                struct Match match;
                                match.pattern_id = pattern_id;
                                match.begin = str;
                                match.end = str + strlen(pattern);
                                return match;
                        }
                }
        }
        return match_empty();
}

struct Match teddy_verify(struct FatTeddy* teddy, __m256i* candidate, char* str, size_t str_size)
{
        __m256i swapped = _mm256_permute4x64_epi64 (*candidate, 0x4e);
        __m256i r1 = _mm256_unpacklo_epi8 (*candidate, swapped);
        __m256i r2 = _mm256_unpackhi_epi8 (*candidate, swapped);

        uint64_t lane = _mm256_extract_epi64 (r1, 0);
        struct Match match = teddy_verify64 (teddy, lane, str, str_size);
        if (match.pattern_id >= 0) {
                return match;
        }
        lane = _mm256_extract_epi64 (r1, 1);
        match = teddy_verify64 (teddy, lane, str, str_size);
        if (match.pattern_id >= 0) {
                return match;
        }
        lane = _mm256_extract_epi64 (r2, 0);
        match = teddy_verify64 (teddy, lane, str, str_size);
        if (match.pattern_id >= 0) {
                return match;
        }
        lane = _mm256_extract_epi64 (r2, 1);
        match = teddy_verify64 (teddy, lane, str, str_size);
        if (match.pattern_id >= 0) {
                return match;
        }
        return match_empty();
}

struct Match teddy_find_next(struct FatTeddy* teddy, char* str, size_t str_size)
{
        __m256i chunk = _mm256_broadcastsi128_si256 (_mm_loadu_si128 ((__m128i*)str));

        __m256i result = lookup_1 (&chunk, &teddy->pattern_mask);

        if (!_mm256_testz_si256 (result, result))
        {
                return teddy_verify(teddy, &result, str, str_size);
        }
        return match_empty();
}

struct Match teddy_find(struct FatTeddy* teddy, char* str, const size_t str_size)
{
        assert (str_size >= 32);

        char* start = str;
        size_t size = str_size;

        while (size >= 32)
        {
                struct Match match = teddy_find_next(teddy, start, size);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                size -= 32;
                start += 32;
        }
        if (size > 0)
        {
                return teddy_find_next(teddy, str + str_size - 33, 32);
        }
        return match_empty();
}

#endif  //SIMD_STRING_TEDDY_H
