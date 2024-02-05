/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#ifndef SIMD_STRING_TIMER_H
#define SIMD_STRING_TIMER_H

#ifdef WIN32

#include <windows.h>

double get_time ();

#else

#include <sys/resource.h>
#include <sys/time.h>

double get_time ();

#endif

#endif  //SIMD_STRING_TIMER_H
