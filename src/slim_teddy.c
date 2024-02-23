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
        memset (self->lo, 0, 32);
        memset (self->hi, 0, 32);

        for (uint8_t bucket_id = 0; bucket_id < 8; ++bucket_id)
        {
                SlimBucket* bucket = &buckets[bucket_id];
                for (uint8_t pidx = 0; pidx < bucket->size; ++pidx)
                {
                        Pattern* pattern = &patterns[pidx];
                        SlimPatternMask_add (self, pattern->begin[self->id], bucket_id);
                }
        }
}

void
SlimPatternMask_add (SlimPatternMask* self, char byte, uint8_t bucket_id)
{
        uint64_t byte_lo = byte & 0xf;
        uint64_t byte_hi = (byte >> 4) & 0xf;
        self->lo[byte_lo] |= 1 << bucket_id;
        self->lo[byte_lo + 16] |= 1 << bucket_id;
        self->hi[byte_hi] |= 1 << bucket_id;
        self->hi[byte_hi + 16] |= 1 << bucket_id;
}

void
SlimPatternMask_build (SlimPatternMask* self)
{
        self->v_lo = _mm_loadu_si128 ((__m128i*) self->lo);
        self->v_hi = _mm_loadu_si128 ((__m128i*) self->hi);
}

void
SlimTeddy_init (SlimTeddy* self, Pattern* patterns, uint8_t num_patterns, uint8_t num_masks)
{
        self->patterns = patterns;
        self->num_patterns = num_patterns;
        self->num_masks = num_masks;

        for (uint8_t bidx = 0; bidx < 8; ++bidx)
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
                        if (current_bucket_id > 8)
                        {
                                // panic
                                assert (0);
                        }
                        current_bucket = &self->buckets[current_bucket_id];
                }
        }

        for (uint8_t mask_idx = 0; mask_idx < self->num_masks; ++mask_idx)
        {
                self->pattern_mask[mask_idx].id = mask_idx;
                SlimPatternMask_init (&self->pattern_mask[mask_idx], self->buckets, self->patterns);
                SlimPatternMask_build (&self->pattern_mask[mask_idx]);
        }
}

Match
SlimTeddy_find (SlimTeddy* self, char* str, size_t str_size)
{
        switch (self->num_masks)
        {
                case 1:
                        return SlimTeddy_find_1(self, str, str_size);
                case 2:
                        return SlimTeddy_find_2 (self, str, str_size);
                case 3:
                        return SlimTeddy_find_3 (self, str, str_size);
                case 4:
                        return SlimTeddy_find_4 (self, str, str_size);
                default:
                        // panic
                        assert(0);
        }
        return Match_empty();
}

Match SlimTeddy_find_1(SlimTeddy* self, char* str, size_t str_size)
{
        assert (str_size >= 16);

        char* cur = str;
        size_t cur_size = str_size;

        while (cur_size >= 16)
        {
                Match match = SlimTeddy_find_one_1 (self, cur, cur_size);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                cur += 16;
                cur_size -= 16;
        }
        if (cur_size > 0)
        {
                cur = str + str_size - 16;
                return SlimTeddy_find_one_1 (self, cur, cur_size);
        }
        return Match_empty();
}

Match SlimTeddy_find_2(SlimTeddy* self, char* str, size_t str_size)
{
        assert (str_size >= 16);

        char* cur = str;
        size_t cur_size = str_size;

        __m128i prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);

        while (cur_size >= 16)
        {
                Match match = SlimTeddy_find_one_2 (self, cur, cur_size, &prev0);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                cur += 16;
                cur_size -= 16;
        }
        if (cur_size > 0)
        {
                prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                cur = str + str_size - 16;
                return SlimTeddy_find_one_2 (self, cur, cur_size, &prev0);
        }
        return Match_empty();
}

Match SlimTeddy_find_3(SlimTeddy* self, char* str, size_t str_size)
{
        assert (str_size >= 16);

        char* cur = str;
        size_t cur_size = str_size;

        __m128i prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);
        __m128i prev1 = _mm_set1_epi8 ((char)(uint8_t)0xff);

        while (cur_size >= 16)
        {
                Match match = SlimTeddy_find_one_3 (self, cur, cur_size, &prev0, &prev1);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                cur += 16;
                cur_size -= 16;
        }
        if (cur_size > 0)
        {
                prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                prev1 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                cur = str + str_size - 16;
                return SlimTeddy_find_one_3 (self, cur, cur_size, &prev0, &prev1);
        }
        return Match_empty();
}

Match SlimTeddy_find_4(SlimTeddy* self, char* str, size_t str_size)
{
        assert (str_size >= 16);

        char* cur = str;
        size_t cur_size = str_size;

        __m128i prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);
        __m128i prev1 = _mm_set1_epi8 ((char)(uint8_t)0xff);
        __m128i prev2 = _mm_set1_epi8 ((char)(uint8_t)0xff);

        while (cur_size >= 16)
        {
                Match match = SlimTeddy_find_one_4 (self, cur, cur_size, &prev0, &prev1, &prev2);
                if (match.pattern_id >= 0)
                {
                        return match;
                }
                cur += 16;
                cur_size -= 16;
        }
        if (cur_size > 0)
        {
                prev0 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                prev1 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                prev2 = _mm_set1_epi8 ((char)(uint8_t)0xff);
                cur = str + str_size - 16;
                return SlimTeddy_find_one_4 (self, cur, cur_size, &prev0, &prev1, &prev2);
        }
        return Match_empty();
}

Match
SlimTeddy_find_one_1 (SlimTeddy* self, char* cur, size_t cur_size)
{
        __m128i chunk = _mm_loadu_si128 ((__m128i*) cur);
        __m128i result;

        mm_lookup_1 (&chunk, self->pattern_mask, &result);

        if (!_mm_testz_si128 (result, result))
        {
                return SlimTeddy_verify (self, &result, cur, cur_size);
        }
        return Match_empty ();
}

Match SlimTeddy_find_one_2 (SlimTeddy* self, char* cur, size_t cur_size, __m128i* prev0)
{
        __m128i chunk = _mm_loadu_si128 ((const __m128i*) cur);
        __m128i result0;
        __m128i result1;

        mm_lookup_2 (&chunk, self->pattern_mask, &result0, &result1);

        __m128i result0prev0 = _mm_alignr_epi8 (result0, *prev0, 15);
        __m128i result = _mm_and_si128 (result0prev0, result1);
        *prev0 = result0;

        if (!_mm_testz_si128 (result, result))
        {
                return SlimTeddy_verify (self, &result, cur, cur_size);
        }
        return Match_empty ();
}
Match SlimTeddy_find_one_3 (SlimTeddy* self, char* cur, size_t cur_size, __m128i* prev0, __m128i* prev1)
{
        __m128i chunk = _mm_loadu_si128 ((const __m128i*) cur);
        __m128i result0;
        __m128i result1;
        __m128i result2;

        mm_lookup_3 (&chunk, self->pattern_mask, &result0, &result1, &result2);

        __m128i result0prev0 = _mm_alignr_epi8 (result0, *prev0, 14);
        __m128i result1prev1 = _mm_alignr_epi8 (result1, *prev1, 15);
        __m128i result = _mm_and_si128 (result0prev0, result1prev1);
        result = _mm_and_si128 (result, result2);
        *prev0 = result0;
        *prev1 = result1;

        if (!_mm_testz_si128 (result, result))
        {
                return SlimTeddy_verify (self, &result, cur, cur_size);
        }
        return Match_empty ();
}

Match SlimTeddy_find_one_4 (SlimTeddy* self, char* cur, size_t cur_size, __m128i* prev0, __m128i* prev1, __m128i* prev2) {
        __m128i chunk = _mm_loadu_si128 ((const __m128i*) cur);
        __m128i result0;
        __m128i result1;
        __m128i result2;
        __m128i result3;

        mm_lookup_4 (&chunk, self->pattern_mask, &result0, &result1, &result2, &result3);

        __m128i result0prev0 = _mm_alignr_epi8 (result0, *prev0, 13);
        __m128i result1prev1 = _mm_alignr_epi8 (result1, *prev1, 14);
        __m128i result2prev2 = _mm_alignr_epi8 (result2, *prev2, 15);
        __m128i result = _mm_and_si128 (result0prev0, result1prev1);
        result = _mm_and_si128 (result, result2prev2);
        result = _mm_and_si128 (result, result3);
        *prev0 = result0;
        *prev1 = result1;
        *prev2 = result2;

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
        uint64_t lanes[2];
        _mm_storeu_epi64 (lanes, *candidate);
        Match match = SlimTeddy_verify64 (self, lanes[0], cur, cur_size);
        if (match.pattern_id >= 0)
        {
                return match;
        }
        cur += 8;
        cur_size -= 8;
        // [64..127]
        match = SlimTeddy_verify64 (self, lanes[1], cur, cur_size);
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
                        if (memcmp (cur, pattern->begin, pattern->size) == 0)
                        {
                                Match match;
                                match.pattern_id = pattern_id;
                                match.begin = cur;
                                match.end = cur + pattern->size;
                                return match;
                        }
                }
        }
        return Match_empty ();
}

void
mm_lookup_1 (__m128i* chunk, SlimPatternMask* masks, __m128i* res0)
{
        const __m128i lo_mask = _mm_set1_epi8 (0xf);
        __m128i chunk_lo = _mm_and_si128 (*chunk, lo_mask);
        __m128i chunk_hi = _mm_srli_epi16 (*chunk, 4);
        chunk_hi = _mm_and_si128 (chunk_hi, lo_mask);

        const __m128i match_lo = _mm_shuffle_epi8 (masks->v_lo, chunk_lo);
        const __m128i match_hi = _mm_shuffle_epi8 (masks->v_hi, chunk_hi);

        *res0 = _mm_and_si128 (match_lo, match_hi);
}

void
mm_lookup_2 (__m128i* chunk, SlimPatternMask* masks, __m128i* res0, __m128i* res1)
{
        const __m128i lo_mask = _mm_set1_epi8 (0xf);
        __m128i chunk_lo = _mm_and_si128 (*chunk, lo_mask);
        __m128i chunk_hi = _mm_srli_epi16 (*chunk, 4);
        chunk_hi = _mm_and_si128 (chunk_hi, lo_mask);

        const __m128i match_lo_0 = _mm_shuffle_epi8 (masks[0].v_lo, chunk_lo);
        const __m128i match_hi_0 = _mm_shuffle_epi8 (masks[0].v_hi, chunk_hi);
        *res0 = _mm_and_si128 (match_lo_0, match_hi_0);

        const __m128i match_lo_1 = _mm_shuffle_epi8 (masks[1].v_lo, chunk_lo);
        const __m128i match_hi_1 = _mm_shuffle_epi8 (masks[1].v_hi, chunk_hi);
        *res1 = _mm_and_si128 (match_lo_1, match_hi_1);
}

void
mm_lookup_3 (__m128i* chunk, SlimPatternMask* masks, __m128i* res0, __m128i* res1, __m128i* res2)
{
        const __m128i lo_mask = _mm_set1_epi8 (0xf);
        __m128i chunk_lo = _mm_and_si128 (*chunk, lo_mask);
        __m128i chunk_hi = _mm_srli_epi16 (*chunk, 4);
        chunk_hi = _mm_and_si128 (chunk_hi, lo_mask);

        const __m128i match_lo_0 = _mm_shuffle_epi8 (masks[0].v_lo, chunk_lo);
        const __m128i match_hi_0 = _mm_shuffle_epi8 (masks[0].v_hi, chunk_hi);
        *res0 = _mm_and_si128 (match_lo_0, match_hi_0);

        const __m128i match_lo_1 = _mm_shuffle_epi8 (masks[1].v_lo, chunk_lo);
        const __m128i match_hi_1 = _mm_shuffle_epi8 (masks[1].v_hi, chunk_hi);
        *res1 = _mm_and_si128 (match_lo_1, match_hi_1);

        const __m128i match_lo_2 = _mm_shuffle_epi8 (masks[2].v_lo, chunk_lo);
        const __m128i match_hi_2 = _mm_shuffle_epi8 (masks[2].v_hi, chunk_hi);
        *res2 = _mm_and_si128 (match_lo_2, match_hi_2);
}

void
mm_lookup_4 (__m128i* chunk, SlimPatternMask* masks, __m128i* res0, __m128i* res1, __m128i* res2, __m128i* res3)
{
        const __m128i lo_mask = _mm_set1_epi8 (0xf);
        __m128i chunk_lo = _mm_and_si128 (*chunk, lo_mask);
        __m128i chunk_hi = _mm_srli_epi16 (*chunk, 4);
        chunk_hi = _mm_and_si128 (chunk_hi, lo_mask);

        const __m128i match_lo_0 = _mm_shuffle_epi8 (masks[0].v_lo, chunk_lo);
        const __m128i match_hi_0 = _mm_shuffle_epi8 (masks[0].v_hi, chunk_hi);
        *res0 = _mm_and_si128 (match_lo_0, match_hi_0);

        const __m128i match_lo_1 = _mm_shuffle_epi8 (masks[1].v_lo, chunk_lo);
        const __m128i match_hi_1 = _mm_shuffle_epi8 (masks[1].v_hi, chunk_hi);
        *res1 = _mm_and_si128 (match_lo_1, match_hi_1);

        const __m128i match_lo_2 = _mm_shuffle_epi8 (masks[2].v_lo, chunk_lo);
        const __m128i match_hi_2 = _mm_shuffle_epi8 (masks[2].v_hi, chunk_hi);
        *res2 = _mm_and_si128 (match_lo_2, match_hi_2);

        const __m128i match_lo_3 = _mm_shuffle_epi8 (masks[3].v_lo, chunk_lo);
        const __m128i match_hi_3 = _mm_shuffle_epi8 (masks[3].v_hi, chunk_hi);
        *res3 = _mm_and_si128 (match_lo_3, match_hi_3);
}