// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#include <simdstr/search.h>
#include <simdstr/utils/utils.h>

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef _MSC_VER
#include <intrin.h>
#include <windows.h>
#else
#include <immintrin.h>
#endif// _MSC_VER

// _____ helper functions _____________________________________________________
// only call, if the str length is too short for simd implementations!

/*
 * Perform strchr with two different chars fst and snd. If fst == snd, strchr
 *  is performed.
 */
const char *
strchrchr (const char *str, size_t str_len, int fst, int snd)
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
rest_strstr (const char *str, size_t str_len, const char *substr, size_t substr_len)
{
        for (/**/; str_len > 0; --str_len)
        {
                bool match = true;
                for (size_t p_i = 0; p_i < substr_len; ++p_i)
                {
                        if (*str != substr[p_i])
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

int
icase_memcmp (const char *str1, const char *str2, size_t size)
{
        for (size_t i = 0; i < size; ++i)
        {
                unsigned char ch1 = tolower ((unsigned char) str1[i]);
                unsigned char ch2 = tolower ((unsigned char) str2[i]);

                if (ch1 < ch2)
                {
                        return -1;
                }
                else if (ch1 > ch2)
                {
                        return 1;
                }
        }

        return 0;// Strings are equal
}

// ____________________________________________________________________________

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
        const __m256i block_last = _mm256_loadu_si256 ((const __m256i *) (str + fst_snd_distance));

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
 /*
inline uint64_t
h_simd_generic_search_64_block_cmp (const char *str, const __m512i fst, const __m512i snd, int fst_snd_distance)
{
        const __m512i block_first = _mm512_loadu_si512 ((const __m256i *) str);
        const __m512i block_last = _mm512_loadu_si512 ((const __m256i *) (str + fst_snd_distance));

        const __mmask64 eq_first = _mm512_cmpeq_epi8_mask (fst, block_first);
        const __mmask64 eq_last = _mm512_cmpeq_epi8_mask (snd, block_last);
        return _kand_mask64 (eq_first, eq_last);
}
*/

/**
 * REMARK: For fst_index > 0, it must be assured that no access to memory < str happens:
 *  assuming mask has no trailing zero, it may happen, that str[-<fst_index>] is accessed.
 *  make sure that the provided *str can handle this!
 */
const char *
h_simd_generic_search_32_mask_cmp (const char *str, const char *substr, size_t substr_len, uint32_t mask, int fst_index)
{
        while (mask != 0)
        {
                const int bitpos = ctz_32 (mask);
                if (memcmp (str + bitpos - fst_index, substr, substr_len) == 0)
                {
                        return str + bitpos - fst_index;
                }
                mask = mask & (mask - 1);
        }
        return NULL;
}

/**
 * REMARK: For fst_index > 0, it must be assured that no access to memory < str happens:
 *  assuming mask has no trailing zero, it may happen, that str[-<fst_index>] is accessed.
 *  make sure that the provided *str can handle this!
 */
 /*
const char *
h_simd_generic_search_64_mask_cmp (const char *str, const char *substr, size_t substr_len, uint64_t mask, int fst_index)
{
        while (mask != 0)
        {
                const int bitpos = ctz_64 (mask);
                if (memcmp (str + bitpos - fst_index, substr, substr_len) == 0)
                {
                        return str + bitpos - fst_index;
                }
                mask = mask & (mask - 1);
        }
        return NULL;
}
*/

const char *
simd_generic_search_32 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index)
{
        if (str == NULL || substr == NULL || substr_len > str_len)
        {
                return NULL;
        }
        fst_index = fst_index < 0 ? 0 : fst_index;
        snd_index = snd_index < 0 ? (int) (substr_len - 1) : snd_index;
        if (fst_index >= substr_len || snd_index >= substr_len || snd_index <= fst_index || snd_index < 1)
        {
                return NULL;
        }

        // perform simd_strchr if pattern size is 1
        if (substr_len == 1)
        {
                return simd_strchr (str, str_len, *substr);
        }

        // ensure proper memory alignment
        if (((uintptr_t) str & 0x1fu) != 0)
        {
                size_t unaligned_size = 32 - (((uintptr_t) str) & 0x1fu);
                const char *result = rest_strstr (str, unaligned_size + substr_len, substr, substr_len);
                if (result != NULL)
                {
                        return result;
                }
                else
                {
                        str += unaligned_size;
                }
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

                const char *match = h_simd_generic_search_32_mask_cmp (str, substr, substr_len, mask, fst_index);
                if (match != NULL)
                {
                        return match;
                }
                str_len -= 32;
                str += 32;
        }
        return strstr (str, substr);
}

/*
const char *
simd_generic_search_64 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index)
{
        if (str == NULL || substr == NULL || substr_len > str_len)
        {
                return NULL;
        }
        fst_index = fst_index < 0 ? 0 : fst_index;
        snd_index = snd_index < 0 ? (int) (substr_len - 1) : snd_index;
        if (fst_index >= substr_len || snd_index >= substr_len || snd_index <= fst_index || snd_index < 1)
        {
                return NULL;
        }

        // perform simd_strchr if pattern size is 1
        if (substr_len == 1)
        {
                return simd_strchr (str, str_len, *substr);
        }

        // ensure proper memory alignment
        if (((uintptr_t) str & 0x3full) != 0)
        {
                size_t unaligned_size = 64 - (((uintptr_t) str) & 0x3full);
                const char *result = rest_strstr (str, unaligned_size + substr_len, substr, substr_len);
                if (result != NULL)
                {
                        return result;
                }
                else
                {
                        str += unaligned_size;
                }
        }

        int fst_snd_distance = snd_index - fst_index;

        // load first char of pattern
        const __m512i first = _mm512_set1_epi8 (substr[fst_index]);
        // load last char of pattern
        const __m512i last = _mm512_set1_epi8 (substr[snd_index]);

        // we are using 256 bit (32 bytes) vectors. If remaining str is smaller than
        // 32, we stop and perform std::strstr on the remaining str
        while (str_len >= 32 + substr_len)
        {
                uint64_t mask = h_simd_generic_search_64_block_cmp (str, first, last, fst_snd_distance);

                const char *match = h_simd_generic_search_64_mask_cmp (str, substr, substr_len, mask, fst_index);
                if (match != NULL)
                {
                        return match;
                }
                str_len -= 32;
                str += 32;
        }
        return strstr (str, substr);
}
*/

// ____________________________________________________________________________

const char *
simd_strchr (const char *str, size_t str_len, int c)
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
                        const unsigned bitpos = ctz_32 (mask);
                        return str + bitpos;
                }
                str_len -= 32;
                str += 32;
        }
        return strchr (str, c);
}

const char *
simd_strichr (const char *str, size_t str_len, int c)
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
                        const unsigned bitpos = ctz_32 (mask);
                        return str + bitpos;
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
        // if (avx512()) {
        //         simd_generic_search_64 (str, str_len, substr, substr_len, -1, -1);
        // } else if (avx2()) {
         return simd_generic_search_32 (str, str_len, substr, substr_len, -1, -1);
        // }
        // return NULL;
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
                        const uint32_t bitpos = ctz_32 (mask);
                        if (icase_memcmp (str + bitpos + 1, substr + 1, substr_len - 2) == 0)
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