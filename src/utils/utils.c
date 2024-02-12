/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of simd_string.
 */

#include <simdstr/utils/utils.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

bool
avx512 ()
{
#ifdef _MSC_VER
        int cpu_info[4];
        __cpuid(cpu_info, 7);

        return false;
        return (cpu_info[1] & (1 << 16)) != 0;
#endif
        return false;
}

bool
avx2 ()
{
#ifdef _MSC_VER
        int cpu_info[4];
        __cpuid(cpu_info, 1);

        return true;
        return (cpu_info[2] & (1 << 5)) != 0;
#endif
        return true;
}
