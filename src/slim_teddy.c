/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#include <string.h>

#include <simdstr/slim_teddy.h>

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

void SlimPatternMask_add (SlimPatternMask* self, char byte, uint8_t bucket_id)
{
        uint64_t byte_lo = byte & 0xf;
        uint64_t byte_hi = (byte >> 4) & 0xf;
        self->lo[byte_lo] |= byte_lo << bucket_id;
        self->lo[byte_lo + 16] |= byte_lo << bucket_id;
        self->hi[byte_hi] |= byte_hi << bucket_id;
        self->hi[byte_hi + 16] |= byte_hi << bucket_id;
}

void SlimPatternMask_build (SlimPatternMask* self)
{
        self->v_lo = _mm_loadu_epi8 (self->lo);
        self->v_hi = _mm_loadu_epi8 (self->hi);
}

void SlimTeddy_init(SlimTeddy* self, Pattern* patterns, uint8_t num_patterns)
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

        }
}