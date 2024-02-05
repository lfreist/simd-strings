// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#include <simdstr/search.h>

#include <ctype.h>
#include <immintrin.h>
#include <stdbool.h>
#include <string.h>

/**
 * Compute bitmask of fst and snd matches in str.
 *  - size of str MUST be >= 32 bytes
 *  - fst MUST NOT be NULL
 *  - snd MUST NOT be NULL
 *  - fst_offset is the offset of char stored in fst to start of substring
 *  - snd_offset is the offset of char stored in snd to start of substring
 *
 *  Example:
 *    a) assuming pattern is "pattern" and fst and snd are the first and the last character in pattern:
 *      - fst = ['p', 'p', ..., 'p']
 *      - snd = ['n', 'n', ..., 'n']
 *      - fst_dst_distance = 6 - 0 = 6
 *    b) assuming pattern is "pattern" and fst is the second and snd is the 4th character in pattern:
 *      - fst = ['a', 'a', ..., 'a']
 *      - snd = ['e', 'e', ..., 'e']
 *      - fst_dst_distance = 4 - 2 = 2
 */
uint32_t
h_simd_generic_search_32_block_cmp (const char *str, const __m256i fst, const __m256i snd, int fst_snd_distance)
{
        const __m256i block_first = _mm256_loadu_si256 ((const __m256i *) str);
        // load 32 bytes stating at offset (pattern_len -1)
        const __m256i block_last = _mm256_loadu_si256 ((const __m256i *) (str + fst_snd_distance));

        // create bit masks
        const __m256i eq_first = _mm256_cmpeq_epi8 (fst, block_first);
        const __m256i eq_last = _mm256_cmpeq_epi8 (snd, block_last);
        return _mm256_movemask_epi8 (_mm256_and_si256 (eq_first, eq_last));
}

/**
 * Compute bitmask of fst and snd matches in str.
 *  - size of str MUST be >= 64 + fst_snd_distance bytes
 *  - fst MUST NOT be NULL
 *  - snd MUST NOT be NULL
 *  - fst_offset is the offset of char stored in fst to start of substring
 *  - snd_offset is the offset of char stored in snd to start of substring
 *
 *  Example:
 *    a) assuming pattern is "pattern" and fst and snd are the first and the last character in pattern:
 *      - fst = ['p', 'p', ..., 'p']
 *      - snd = ['n', 'n', ..., 'n']
 *      - fst_dst_distance = 6 - 0 = 6
 *    b) assuming pattern is "pattern" and fst is the second and snd is the 4th character in pattern:
 *      - fst = ['a', 'a', ..., 'a']
 *      - snd = ['e', 'e', ..., 'e']
 *      - fst_dst_distance = 4 - 2 = 2
 */
uint64_t
h_simd_generic_search_64_block_cmp (const char *str, const __m512i fst, const __m512i snd, int fst_snd_distance)
{
        const __m512i block_first = _mm512_loadu_si512 ((const __m256i *) str);
        // load 32 bytes stating at offset (pattern_len -1)
        const __m512i block_last = _mm512_loadu_si512 ((const __m256i *) (str + fst_snd_distance));

        // create bit masks
        const __mmask64 eq_first = _mm512_cmpeq_epi8_mask (fst, block_first);
        const __mmask64 eq_last = _mm512_cmpeq_epi8_mask (snd, block_last);
        return _kand_mask64 (eq_first, eq_last);
}

const char *
h_simd_generic_search_32_mask_cmp (const char* str, const char* substr, size_t substr_len, uint32_t mask, int fst_index, int snd_index)
{
        switch (fst_index)
        {
                case 0:
                        while (mask != 0)
                        {
                                const uint32_t bitpos = __builtin_ctz (mask);
                                if (memcmp (str + bitpos + 1, substr + 1, substr_len - 1) == 0)
                                {
                                        return str + bitpos;
                                }
                                mask = mask & (mask - 1);
                        }
                        return NULL;
                case 1:
                        while (mask != 0)
                        {
                                const uint32_t bitpos = __builtin_ctz (mask);
                                if (((str[bitpos - 1]) == substr[0]) && memcmp (str + bitpos + 1 + fst_index, substr + 1, substr_len - snd_index) == 0)
                                {
                                        return str + bitpos;
                                }
                                mask = mask & (mask - 1);
                        }
                        return NULL;
                default:
        }
        if (fst_index == 0)
        {

        } else if ()
}

const char *
h_simd_generic_search_64_mask_cmp (const char* str, const char* substr, size_t substr_len, uint64_t mask);

const char *
simd_generic_search_32 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index)
{
        if (str == NULL || substr == NULL || substr_len > str_len)
        {
                return NULL;
        }
        if (fst_index < 0)
        {
                fst_index = 0;
        }
        if (snd_index < 0)
        {
                snd_index = (int) (substr_len - 1);
        }
        if (fst_index >= substr_len || snd_index >= substr_len || snd_index <= fst_index || fst_index < 0 || snd_index < 1)
        {
                return NULL;
        }
        if (substr_len == 1)
        {
                return simd_strchr (str, str_len, substr[0]);
        }
        if (str_len < 32 + substr_len)
        {
                return strstr (str, substr);
        }

        int fst_snd_distance = snd_index - fst_index;

        // load first char of pattern
        const __m256i first = _mm256_set1_epi8 (substr[fst_index]);
        // load last char of pattern
        const __m256i last = _mm256_set1_epi8 (substr[snd_index]);

        // we are using 256 bits (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strstr on the remaining str
        while (str_len >= 32 + substr_len)
        {
                uint32_t mask = h_simd_generic_search_32_block_cmp (str, first, last, fst_snd_distance);

                while (mask != 0)
                {
                        // get index of first 1 starting at least significant bit for little endian
                        // TODO: does endianess matter here?
                        const uint32_t bitpos = __builtin_ctz (mask);
                        if (memcmp (str + bitpos + 1, substr + 1, substr_len - 2) == 0)
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

// _____ helper functions _____________________________________________________
// only call, if the str length is too short for simd implementations!

/*
 * Perform strchr with two different chars fst and snd. If fst == snd, strchr
 *  is performed.
 */
const char *
strchrchr (const char *str, size_t str_len, char fst, char snd)
{
        if (fst == snd)
        {
                return strchr (str, fst);
        }

        for (size_t i = 0; i < str_len; ++i)
        {
                if (fst == str[i] || snd == str[i])
                        return str + i;
        }
        return NULL;
}

const char *
rest_stristr (const char *str, size_t str_len, const char *substr, size_t substr_len)
{
        for (/**/; str_len > 0; --str_len)
        {
                bool match = true;
                for (size_t p_i = 0; p_i < substr_len; ++p_i)
                {
                        if (tolower (*str) != tolower (substr[p_i]))
                        {
                                match = false;
                                break;
                        }
                        str++;
                }
                if (match)
                {
                        return str - substr_len;
                }
                str++;
        }
        return NULL;
}

// ____________________________________________________________________________

const char *
simd_strchr (const char *str, size_t str_len, char c)
{
        if (str == NULL || str_len < 1)
                return NULL;
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
simd_strichr (const char *str, size_t str_len, char c)
{
        if (str == NULL || str_len < 1)
                return NULL;
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
        if (str == NULL || substr == NULL || substr_len > str_len)
                return NULL;
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
                        if (memcmp (str + bitpos + 1, substr + 1, substr_len - 2) == 0)
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

const char *
simd_stristr (const char *str, size_t str_len, const char *substr, size_t substr_len)
{
        if (str == NULL || substr == NULL || substr_len > str_len)
                return NULL;
        if (substr_len == 1)
                return simd_strichr (str, str_len, substr[0]);
        if (str_len < 32 + substr_len)
                return rest_stristr (str, str_len, substr, substr_len);

        const __m256i first_lower = _mm256_set1_epi8 ((char) tolower (substr[0]));
        const __m256i first_upper = _mm256_set1_epi8 ((char) toupper (substr[0]));
        const __m256i last_lower = _mm256_set1_epi8 ((char) tolower (substr[substr_len - 1]));
        const __m256i last_upper = _mm256_set1_epi8 ((char) toupper (substr[substr_len - 1]));

        // we are using 256 bits (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strstr on the remaining str
        while (str_len >= 32 + substr_len)
        {
                // load next 32 bytes
                const __m256i block_first = _mm256_loadu_si256 ((const __m256i *) str);
                // load 32 bytes stating at offset (pattern_len -1)
                const __m256i block_last = _mm256_loadu_si256 ((const __m256i *) (str + substr_len - 1));

                // create bit masks
                const __m256i eq_first_lower = _mm256_cmpeq_epi8 (first_lower, block_first);
                const __m256i eq_first_upper = _mm256_cmpeq_epi8 (first_upper, block_first);
                const __m256i eq_last_lower = _mm256_cmpeq_epi8 (last_lower, block_last);
                const __m256i eq_last_upper = _mm256_cmpeq_epi8 (last_upper, block_last);

                // use bitwise or for lower and upper vector
                const __m256i eq_first = _mm256_or_si256 (eq_first_lower, eq_first_upper);
                const __m256i eq_last = _mm256_or_si256 (eq_last_lower, eq_last_upper);

                uint32_t mask = _mm256_movemask_epi8 (_mm256_and_si256 (eq_first, eq_last));

                while (mask != 0)
                {
                        // get index of first 1 starting at least significant bit for little endian
                        // TODO: does endianess matter here?
                        //  -> Probably not because this stuff is x86 specific which always is little endian?!
                        const uint32_t bitpos = __builtin_ctz (mask);
                        if (strncasecmp (str + bitpos + 1, substr + 1, substr_len - 2) == 0)
                        {
                                return str + bitpos;
                        }
                        mask = mask & (mask - 1);// turn least significant 1 to 0
                        // maybe this: this bitshifts mask so that the least significant 1 is gone.
                        //  We now only need to add the new __builtin_ctz to the old value to get the new shift.
                        // TODO: test this if it works and if it is faster
                        // mask >>= bitpos;
                }
                str_len -= 32;
                str += 32;
        }
        return rest_stristr (str, str_len, substr, substr_len);
}