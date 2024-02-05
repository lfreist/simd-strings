/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of simd_string.
 */

#include <simdstr/utils/utils.h>

#include <intrin.h>

bool
avx512 ()
{
        int cpu_info[4];
        __cpuid(cpu_info, 7);

        return (cpu_info[1] & (1 << 16)) != 0;
}

bool
avx2 ()
{
        int cpu_info[4];
        __cpuid(cpu_info, 1);

        return (cpu_info[2] & (1 << 5)) != 0;
}
