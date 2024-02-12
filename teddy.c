/**
* Copyright 2024, Leon Freist (https://github.com/lfreist)
* Author: Leon Freist <freist.leon@gmail.com>
*
* This file is part of simd_string.
*/

#include <immintrin.h>

#include <stdint.h>
#include <stdio.h>

int main()
{
        char haystack[] = "bat cat foo bump";
        char* patterns[] = {"foo", "bar", "bat"};

        __m128i mask = _mm_set1_epi8 (0xf);

        __m128i block = _mm_loadu_epi8 (&haystack[0]);

        __m128i B0 = _mm_and_si128 (block, mask);
        __m128i B1 = _mm_srli_epi16 (block, 4);
        B1 = _mm_and_si128 (B1, mask);

        uint8_t p_mask_lo[32] = {0};
        uint8_t p_mask_hi[32] = {0};

        for (int i = 0; i < 3; ++i)
        {
                char byte = patterns[i][0];
                uint64_t byte_lo = byte & 0xf;
                uint64_t byte_hi = ((byte >> 4) & 0xf);
                p_mask_lo[byte_lo] |= 1 << i;
                p_mask_lo[byte_lo + 16] |= 1 << i;
                p_mask_hi[byte_hi] |= 1 << i;
                p_mask_hi[byte_hi + 16] |= 1 << i;
        }

        __m128i A0 = _mm_loadu_epi8 ((__m128i*)p_mask_lo);
        __m128i A1 = _mm_loadu_epi8 ((__m128i*)p_mask_hi);

        __m128i C0 = _mm_shuffle_epi8 (A0, B0);
        __m128i C1 = _mm_shuffle_epi8 (A1, B1);

        __m128i C = _mm_and_si128 (C0, C1);

        uint8_t c_bytes[16] = {0};
        _mm_storeu_si128 ((__m128i*)c_bytes, C);

        for (int i = 0; i < 16; ++i)
        {
                if (c_bytes[i] != 0)
                {
                        printf ("Possible match: %s\n", &haystack[i]);
                        // count leading zeros
                        for (int j = 0; j < 8; ++j)
                        {
                                if ((c_bytes[i] & (1 << j)) != 0)
                                {
                                        printf (" -> Possible pattern: %s\n", patterns[j]);
                                }
                        }
                }
        }

        return 0;
}