/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#ifndef SIMD_STRING_STATISTICS_H
#define SIMD_STRING_STATISTICS_H

double mean (double *data, int size);

double stddev (double *data, int size, double mean);

double median (double *data, int size);

#endif//SIMD_STRING_STATISTICS_H
