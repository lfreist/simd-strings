// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#include <simdstr/search/search.h>

#include <ctype.h>
#include <immintrin.h>
#include <string.h>

// _____ helper functions _____________________________________________________
/*
 * Perform strchr with two different chars fst and snd.
 *
 * Mark: there is no optimization for the case that fst == snd
 */
const char *
strchrchr (const char *str, size_t str_len, int fst, int snd)
{
        for (size_t i = 0; i < str_len; ++i)
        {
                if (fst == str[i] || snd == str[i])
                        return str + i;
        }
        return NULL;
}

// ____________________________________________________________________________

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
                        _mm256_loadu_si256 ((const __m256i *) (str));

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

const char *
simd_strichr (const char *str, size_t str_len, int c)
{
        int lower = tolower (c);
        int upper = toupper (c);
        if (lower == upper)
                return simd_strchr (str, str_len, c);
        if (str_len < 32)
        {
                return strchrchr (str, str_len, lower, upper);
        }
        // load c into SIMD vector
        const __m256i _lower = _mm256_set1_epi8 ((char) lower);
        const __m256i _upper = _mm256_set1_epi8 ((char) upper);

        // we are using 256 bits (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strchr on the remaining str
        while (str_len >= 32)
        {
                // load next 32 bytes
                const __m256i block =
                        _mm256_loadu_si256 ((const __m256i *) (str));

                const __m256i lower_eq = _mm256_cmpeq_epi8 (_lower, block);
                const __m256i upper_eq = _mm256_cmpeq_epi8 (_upper, block);
                const __m256i eq = _mm256_or_si256 (lower_eq, upper_eq);

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
        return strchrchr (str, str_len, lower, upper);
}

const char *
simd_strstr (const char *str, size_t str_len, const char *substr, size_t substr_len)
{
        if (substr_len == 1)
                return simd_strchr (str, str_len, substr[0]);
        if (str_len < 32 + substr_len)
                return strstr (str, substr);

        // load first char of pattern
        const __m256i first = _mm256_set1_epi8 (substr[0]);
        // load last char of pattern
        const __m256i last = _mm256_set1_epi8 (substr[substr_len - 1]);

        // we are using 256 bits (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strstr on the remaining str
        while (str_len >= 32 + substr_len)
        {
                // load next 32 bytes
                const __m256i block_first = _mm256_loadu_si256 ((const __m256i *) str);
                // load 32 bytes stating at offset (pattern_len -1)
                const __m256i block_last = _mm256_loadu_si256 ((const __m256i *) (str + substr_len - 1));

                // create bit masks
                const __m256i eq_first = _mm256_cmpeq_epi8 (first, block_first);
                const __m256i eq_last = _mm256_cmpeq_epi8 (last, block_last);
                uint32_t mask = _mm256_movemask_epi8 (_mm256_and_si256 (eq_first, eq_last));

                while (mask != 0)
                {
                        // get index of first 1 starting at least significant bit for little endian
                        // TODO: does endianess matter here?
                        const uint32_t bitpos = __builtin_ctz (mask);
                        if (memcmp (str + bitpos + 1, substr + 1, substr_len - 1) == 0)
                        {
                                return str + bitpos;
                        }
                        mask = mask & (mask - 1);// turn least significant 1 to 0
                }
                str_len -= 32;
                str += 32;
        }
        return strstr (str, substr);
}