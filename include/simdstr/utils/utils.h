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
#include <windows.h>
#endif

bool avx512 ();

bool avx2 ();

inline int
ctz_32 (uint32_t value)
{
#ifdef _MSC_VER
        DWORD trailing_zeroes = 0;

        if (_BitScanForward (&trailing_zeroes, value))
        {
                return (int) trailing_zeroes;
        }
        else
        {
                // This is undefined, I better choose 32 than 0
                return 32;
        }
#else
        return __builtin_ctz (value);
#endif
}

inline int
ctz_64 (uint64_t value)
{
#ifdef _MSC_VER
        DWORD trailing_zeroes = 0;

        if (_BitScanForward (&trailing_zeroes, value))
        {
                return (int) trailing_zeroes;
        }
        else
        {
                // This is undefined, I better choose 32 than 0
                return 32;
        }
#else
        return __builtin_ctzll (value);
#endif
}

#endif  //SIMD_STRING_UTILS_H
