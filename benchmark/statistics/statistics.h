/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#ifndef SIMD_STRING_STATISTICS_H
#define SIMD_STRING_STATISTICS_H

double mean (const double *data, int size);

double stddev (const double *data, int size, double mean);

double median (const double *data, int size);

double min_a(const double* data, int size);

double max_a(const double* data, int size);

#endif//SIMD_STRING_STATISTICS_H
