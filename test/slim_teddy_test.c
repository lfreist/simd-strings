/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include "minunit.h"

#include <simdstr/slim_teddy.h>

SlimTeddy teddy;

MU_TEST (init_test_no_patterns)
{
        SlimTeddy_init (&teddy, NULL, 0);

        SlimPatternMask mask;
        SlimPatternMask_init (&mask, teddy.buckets, teddy.patterns);
}
