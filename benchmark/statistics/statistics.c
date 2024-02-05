/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "statistics.h"

double
mean (double *data, int size)
{
        double sum = 0.0;
        for (int i = 0; i < size; ++i)
        {
                sum += data[i];
        }
        return sum / size;
}

double
stddev (double *data, int size, double mean)
{
        double sum = 0.0;
        for (int i = 0; i < size; ++i)
        {
                sum += (data[i] - mean) * (data[i] - mean);
        }
        return sqrt (sum / size);
}

int
compare_doubles (const void *a, const void *b)
{
        return (*(double *) a - *(double *) b);
}

double
median (double *data, int size)
{
        double *sorted_data = malloc (size * sizeof (double));
        memcpy (sorted_data, data, size * sizeof (double));

        qsort (sorted_data, size, sizeof (double), compare_doubles);

        double median_value;

        if (size % 2 == 0)
        {
                median_value = (sorted_data[size / 2 - 1] + sorted_data[size / 2]) / 2.0;
        }
        else
        {
                median_value = sorted_data[size / 2];
        }

        free (sorted_data);
        return median_value;
}