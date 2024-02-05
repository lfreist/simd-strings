/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statistics/statistics.h"
#include "timer/timer.h"

#include <simdstr/search.h>

struct Result_T {
        double mean;
        double stddev;
        double median;
        long position;
};

struct Result_T
benchmark_strchr (const char *haystack, int c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start = get_time ();
                const char *result = strchr (haystack, c);
                double end = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end - start;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strchr (const char *haystack, size_t size, char c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start = get_time ();
                const char *result = simd_strchr (haystack, size, c);
                double end = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end - start;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strichr (const char *haystack, size_t size, char c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start = get_time ();
                const char *result = simd_strichr (haystack, size, c);
                double end = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end - start;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_strstr (const char *haystack, const char *needle, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start_time = get_time ();
                const char *result = strstr (haystack, needle);
                double end_time = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end_time - start_time;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strstr (const char *haystack, size_t str_size, const char *needle, size_t needle_size, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start_time = get_time ();
                const char *result = simd_strstr (haystack, str_size, needle, needle_size);
                double end_time = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end_time - start_time;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_stristr (const char *haystack, size_t str_size, const char *needle, size_t needle_size, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start_time = get_time ();
                const char *result = simd_stristr (haystack, str_size, needle, needle_size);
                double end_time = get_time ();
                if (position < 0 && result != NULL)
                {
                        position = result - haystack;
                }
                times[i] = end_time - start_time;
        }

        struct Result_T result;
        result.mean = mean (times, iterations);
        result.stddev = stddev (times, iterations, result.mean);
        result.median = median (times, iterations);
        result.position = position;

        free (times);
        return result;
}

char *
read_file (const char *path)
{
        FILE *file = fopen (path, "r");

        if (file == NULL)
        {
                perror ("Error opening file");
                return NULL;
        }

        fseek (file, 0, SEEK_END);
        long fileSize = ftell (file);
        fseek (file, 0, SEEK_SET);

        char *fileContent = malloc ((fileSize + 1) * sizeof (char));

        if (fileContent == NULL)
        {
                perror ("Error allocating memory");
                fclose (file);
                return NULL;
        }

        size_t bytesRead = fread (fileContent, 1, fileSize, file);
        fileContent[bytesRead] = '\0';

        fclose (file);

        return fileContent;
}

int
main (int argc, char **argv)
{
        if (argc != 3)
        {
                return 1;
        }

        const char *file_path = argv[1];
        const char *needle = argv[2];

        char *haystack = read_file (file_path);

        int iterations = 5;

        printf ("Needle:  \"%s\"\n", needle);
        printf ("Textsize: %lu\n", strlen (haystack));

        printf ("\nBenchmarking simd_strstr...\n");
        struct Result_T result_simd_strstr = benchmark_simd_strstr (haystack, strlen (haystack), needle, strlen (needle), iterations);
        printf (" Position: %ld\n\n", result_simd_strstr.position);
        printf (" Mean:     %f\n", result_simd_strstr.mean);
        printf (" StdDev:   %f\n", result_simd_strstr.stddev);
        printf (" Median:   %f\n\n", result_simd_strstr.median);

        printf ("Benchmarking strstr...\n");
        struct Result_T result_strstr = benchmark_strstr (haystack, needle, iterations);
        printf (" Position: %ld\n\n", result_strstr.position);
        printf (" Mean:     %f\n", result_strstr.mean);
        printf (" StdDev:   %f\n", result_strstr.stddev);
        printf (" Median:   %f\n", result_strstr.median);

        free(haystack);

        return 0;
}