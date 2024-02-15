/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of simd_string.
 */

#ifndef SIMD_STRING_UTILS_H
#define SIMD_STRING_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef _MSC_VER
#include <intrin.h>
#include <windows.h>
#endif

extern inline bool
avx512 ()
{
        return false;
}

extern inline bool
avx2 ()
{
        return true;
}

extern inline uint32_t
ctz_32 (uint32_t value)
{
#ifdef _MSC_VER
        return _tzcnt_u32(value);
#else
        return (uint32_t)__builtin_ctz (value);
#endif
}

extern inline uint64_t
ctz_64 (uint64_t value)
{
#ifdef _MSC_VER
        return _tzcnt_u64(value);
#else
        return (uint64_t)__builtin_ctzll (value);
#endif
}

#endif//SIMD_STRING_UTILS_H
