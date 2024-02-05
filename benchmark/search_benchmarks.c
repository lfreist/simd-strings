/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

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
        double min;
        double max;
        long long position;
};

struct Result_T
benchmark_strchr (const char *haystack, int c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strchr (const char *haystack, size_t size, char c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strichr (const char *haystack, size_t size, char c, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_strstr (const char *haystack, const char *needle, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strstr_64 (const char *haystack, size_t str_size, const char *needle, size_t needle_size, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start_time = get_time ();
                const char *result = simd_generic_search_64 (haystack, str_size, needle, needle_size, -1, -1);
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_strstr_32 (const char *haystack, size_t str_size, const char *needle, size_t needle_size, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
        for (int i = 0; i < iterations; ++i)
        {
                double start_time = get_time ();
                const char *result = simd_generic_search_32 (haystack, str_size, needle, needle_size, -1, -1);
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
        result.position = position;

        free (times);
        return result;
}

struct Result_T
benchmark_simd_stristr (const char *haystack, size_t str_size, const char *needle, size_t needle_size, int iterations)
{
        double *times = malloc (iterations * sizeof (double));
        long long position = -1;
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
        result.min = min_a (times, iterations);
        result.max = max_a (times, iterations);
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

void
print_result (const struct Result_T *result)
{
        printf (" Position: %lld\n\n", result->position);
        printf (" result:   %f +/- %f ms\n", result->mean * 1000.0f, result->stddev * 1000.0f);
        printf (" Min:      %f ms\n", result->min * 1000.0f);
        printf (" Max:      %f ms\n", result->max * 1000.0f);
        printf (" Median:   %f ms\n\n", result->median * 1000.0f);
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
        printf ("Textsize: %llu\n", strlen (haystack));

        printf ("Benchmarking strstr...\n");
        struct Result_T result_strstr = benchmark_strstr (haystack, needle, iterations);
        print_result (&result_strstr);

        printf ("\nBenchmarking simd_strstr_32...\n");
        struct Result_T result_simd_strstr_32 = benchmark_simd_strstr_32 (haystack, strlen (haystack), needle, strlen (needle), iterations);
        print_result (&result_simd_strstr_32);

        printf ("\nBenchmarking simd_strstr_64...\n");
        struct Result_T result_simd_strstr_64 = benchmark_simd_strstr_64 (haystack, strlen (haystack), needle, strlen (needle), iterations);
        print_result ((&result_simd_strstr_64));

        free (haystack);

        return 0;
}