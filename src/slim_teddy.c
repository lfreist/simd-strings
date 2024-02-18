/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#include <assert.h>
#include <emmintrin.h>
#include <string.h>

#include <simdstr/slim_teddy.h>
#include <simdstr/utils/utils.h>

void
SlimPatternMask_init (SlimPatternMask* self, SlimBucket* buckets, Pattern* patterns)
{
        memset (self->lo, 0, 16);
        memset (self->hi, 0, 16);

        for (uint8_t bucket_id = 0; bucket_id < 8; ++bucket_id)
        {
                SlimBucket* bucket = &buckets[bucket_id];
                for (uint8_t pidx = 0; pidx < bucket->size; ++pidx)
                {
                        Pattern* pattern = &patterns[pidx];
                        SlimPatternMask_add (self, pattern->start[0], bucket_id);
                }
        }
}

void
SlimPatternMask_add (SlimPatternMask* self, char byte, uint8_t bucket_id)
{
        uint64_t byte_lo = byte & 0xf;
        uint64_t byte_hi = (byte >> 4) & 0xf;
        self->lo[byte_lo] |= byte_lo << bucket_id;
        self->lo[byte_lo + 16] |= byte_lo << bucket_id;
        self->hi[byte_hi] |= byte_hi << bucket_id;
        self->hi[byte_hi + 16] |= byte_hi << bucket_id;
}

void
SlimPatternMask_build (SlimPatternMask* self)
{
        self->v_lo = _mm_loadu_si128 ((__m128i*) self->lo);
        self->v_hi = _mm_loadu_si128 ((__m128i*) self->hi);
}

void
SlimTeddy_init (SlimTeddy* self, Pattern* patterns, uint8_t num_patterns)
{
        self->patterns = patterns;
        self->num_patterns = num_patterns;

        for (uint8_t bidx = 0; bidx < 16; ++bidx)
        {
                self->buckets[bidx].size = 0;
        }

        SlimBucket* current_bucket = &self->buckets[0];
        uint8_t current_bucket_id = 0;

        for (uint8_t pattern_id = 0; pattern_id < self->num_patterns; ++pattern_id)
        {
                current_bucket->pattern_ids[current_bucket->size] = pattern_id;
                current_bucket->size++;

                if (current_bucket->size == 8)
                {
                        current_bucket_id++;
                        if (current_bucket_id > 16)
                        {
                                // panic
                                assert (0);
                        }
                        current_bucket = &self->buckets[current_bucket_id];
                }
        }

        SlimPatternMask_init (&self->pattern_mask, self->buckets, self->patterns);
        SlimPatternMask_build (&self->pattern_mask);
}

Match
SlimTeddy_find (SlimTeddy* self, char* str, size_t str_size)
{
        assert (str_size >= 16);

        char* cur = str;
        size_t cur_size = str_size;

        while (cur_size >= 16)
        {
                Match match = SlimTeddy_find_one (self, cur, cur_size);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                cur_size -= 16;
                cur += 16;
        }
        if (cur_size > 0)
        {
                // search last 16 bytes
                return SlimTeddy_find_one (self, str + str_size - 16, 16);
        }
        return Match_empty ();
}

Match
SlimTeddy_find_one (SlimTeddy* self, char* cur, size_t cur_size)
{
        __m128i chunk = _mm_loadu_si128 ((const __m128i*) cur);
        __m128i result = lookup_1 (&chunk, &self->pattern_mask);

        if (!_mm_testz_si128 (result, result))
        {
                return SlimTeddy_verify (self, &result, cur, cur_size);
        }
        return Match_empty ();
}

Match
SlimTeddy_verify (SlimTeddy* self, __m128i* candidate, char* cur, size_t cur_size)
{
        // [0..63]
        uint64_t lane = _mm_extract_epi64 (*candidate, 0);
        Match match = SlimTeddy_verify64 (self, lane, cur, cur_size);
        if (match.pattern_id >= 0)
        {
                return match;
        }
        cur += 8;
        // [64..127]
        lane = _mm_extract_epi64 (*candidate, 1);
        match = SlimTeddy_verify64 (self, lane, cur, cur_size);
        if (match.pattern_id >= 0)
        {
                return match;
        }
        return Match_empty ();
}

Match
SlimTeddy_verify64 (SlimTeddy* self, uint64_t lane, char* cur, size_t cur_size)
{
        while (lane != 0)
        {
                uint64_t bit = ctz_64 (lane);
                lane &= ~((uint64_t) (1) << bit);

                uint64_t shift = (bit / 8);
                if (shift > cur_size)
                {
                        break;
                }
                cur_size -= shift;
                cur += shift;

                SlimBucket* bucket = &self->buckets[bit % 8];

                for (uint8_t pidx = 0; pidx < bucket->size; ++pidx)
                {
                        uint8_t pattern_id = bucket->pattern_ids[pidx];
                        Pattern* pattern = &self->patterns[pattern_id];
                        if (cur_size < pattern->size)
                        {
                                continue;
                        }
                        if (memcmp (cur, pattern->start, pattern->size) == 0)
                        {
                                Match match;
                                match.pattern_id = pattern_id;
                                match.start = cur;
                                match.end = cur + pattern->size;
                                return match;
                        }
                }
        }
        return Match_empty ();
}

__m128i
lookup_1 (__m128i* chunk, SlimPatternMask* mask)
{
        const __m128i lo_mask = _mm_set1_epi8 (0xf);
        __m128i chunk_lo = _mm_and_si128 (*chunk, lo_mask);
        __m128i chunk_hi = _mm_srli_epi16 (*chunk, 4);
        chunk_hi = _mm_and_si128 (chunk_hi, lo_mask);

        const __m128i match_lo = _mm_shuffle_epi8 (mask->v_lo, chunk_lo);
        const __m128i match_hi = _mm_shuffle_epi8 (mask->v_hi, chunk_hi);

        return _mm_and_si128 (match_lo, match_hi);
}