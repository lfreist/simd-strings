// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#ifndef SIMDSTR_H_
#define SIMDSTR_H_

#include <stddef.h>
#include <stdint.h>

const char *
simd_generic_search_32 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index);

const char *
simd_generic_search_64 (const char *str, size_t str_len, const char *substr, size_t substr_len, int fst_index, int snd_index);

const char *
simd_strchr (const char *str, size_t str_len, int c);

const char *
simd_strichr (const char *str, size_t str_len, int c);

const char *
simd_strstr (const char *str, size_t str_len, const char *substr, size_t substr_len);

const char *
simd_stristr (const char *str, size_t str_len, const char *substr, size_t substr_len);

#endif  // SIMDSTR_H_
