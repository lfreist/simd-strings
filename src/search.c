// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#include "include/simdstr/search.h"

#include <immintrin.h>
#include <string.h>

// _____ helper functions _____________________________________________________

const char *
simd_strchr (const char *str, size_t str_len, int c)
{
        if (str_len < 32)
        {
                return strchr (str, c);
        }
        // load c into SIMD vector
        const __m256i _c = _mm256_set1_epi8 (c);

        // we are using 256 bits (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strchr on the remaining str
        while (str_len >= 32)
        {
                // load next 32 bytes
                const __m256i block =
                        _mm256_loadu_si256 (reinterpret_cast<const __m256i *> (str));

                const __m256i eq = _mm256_cmpeq_epi8 (_c, block);

                // create mask
                uint32_t mask = _mm256_movemask_epi8 (eq);
                if (mask != 0)
                {
                        const unsigned bitpos =
                                __builtin_ctz (mask);// get index of first true value
                        return str + bitpos;         // return position of match
                }
                str_len -= 32;
                str += 32;
        }
        // perform rest_strchr on remaining str
        return strchr (str, c);
}