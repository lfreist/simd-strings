// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#ifndef SIMDSTR_H_
#define SIMDSTR_H_

#include <stddef.h>
#include <stdint.h>

/**
 * Generic SIMD substring search using AVX2 instructions with 256 bit registers.
 */
const char *
simd_generic_search_avx_32 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index);

/**
 * Generic SIMD substring search using AVX512 instructions with 512 bit registers.
 */
const char *
simd_generic_search_avx_64 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index);

/**
 * AVX2 based SIMD implementation of strchr using 256 bit registers.
 */
const char *
simd_strchr (const char *str, size_t str_len, int c);

/**
 * AVX2 based SIMD implementation of case insensitive strchr using 256 bit registers.
 */
const char *
simd_strichr (const char *str, size_t str_len, int c);

/**
 * SIMD based strstr implementation using available SIMD instruction sets with fallback to clib's strstr if no SIMD instructions are available.
 */
const char *
simd_strstr (const char *str, size_t str_len, const char *substr, size_t substr_len);

/**
 * SIMD based case insensitive strstr implementation using available SIMD instruction sets with fallback to standard implementation if no SIMD
 *  instructions are available.
 */
const char *
simd_stristr (const char *str, size_t str_len, const char *substr, size_t substr_len);

#endif  // SIMDSTR_H_
