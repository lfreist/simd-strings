// Copyright 2023, Leon Freist
// Author: Leon Freist <freist.leon@gmail.com>

#ifndef SIMDSTR_SEARCH_H_
#define SIMDSTR_SEARCH_H_

#include <stdint.h>

const char *
simd_strchr (const char *str, size_t str_len, int c);

const char *
simd_strstr (const char *str, size_t str_len, const char *substr, size_t substr_len);

const char *
simd_stristr (const char *str, size_t str_len, const char *substr, size_t substr_len)

#endif// SIMDSTR_SEARCH_H_
