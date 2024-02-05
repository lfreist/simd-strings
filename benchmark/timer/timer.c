/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include "timer.h"

#ifdef WIN32

double
get_time ()
{
        LARGE_INTEGER t, f;
        QueryPerformanceCounter (&t);
        QueryPerformanceFrequency (&f);
        return (double) t.QuadPart / (double) f.QuadPart;
}

#else

double
get_time ()
{
        struct timeval t;
        struct timezone tzp;
        gettimeofday (&t, &tzp);
        return t.tv_sec + t.tv_usec * 1e-6;
}

#endif
