#include "minunit.h"
#include <simdstr/search/search.h>
#include <string.h>

MU_TEST(test_empty_text) {
        const char *text = "";
        size_t text_size = 0;
        const char *pattern = "test";
        size_t pattern_size = 4;
        mu_check(simd_strstr(text, text_size, pattern, pattern_size) == NULL);
}

MU_TEST(test_empty_pattern) {
        const char *text = "This is a text.";
        size_t text_size = 15;
        const char *pattern = "";
        size_t pattern_size = 0;
        mu_assert_string_eq(text, simd_strstr(text, text_size, pattern, pattern_size));
}

MU_TEST(test_null_text) {
        const char *text = NULL;
        size_t text_size = 10;
        const char *pattern = "test";
        size_t pattern_size = 4;
        mu_check(simd_strstr(text, text_size, pattern, pattern_size) == NULL);
}

MU_TEST(test_null_pattern) {
        const char *text = "This is a text.";
        size_t text_size = 15;
        const char *pattern = NULL;
        size_t pattern_size = 4;
        mu_check(simd_strstr(text, text_size, pattern, pattern_size) == NULL);
}

MU_TEST(test_pattern_larger_than_text) {
        const char *text = "test";
        size_t text_size = 4;
        const char *pattern = "This is a larger pattern.";
        size_t pattern_size = 26;
        mu_check(simd_strstr(text, text_size, pattern, pattern_size) == NULL);
}

MU_TEST(test_pattern_at_end_of_text) {
        const char *text = "This is a text with pattern";
        size_t text_size = 30;
        const char *pattern = "pattern";
        size_t pattern_size = 7;
        mu_assert_string_eq(text + 20, simd_strstr(text, text_size, pattern, pattern_size));
}

MU_TEST(test_pattern_at_start_of_text) {
        const char *text = "pattern in the text";
        size_t text_size = 20;
        const char *pattern = "pattern";
        size_t pattern_size = 7;
        mu_assert_string_eq(text, simd_strstr(text, text_size, pattern, pattern_size));
}

MU_TEST(test_pattern_in_the_middle_of_text) {
        const char *text = "This is a pattern in the middle";
        size_t text_size = 32;
        const char *pattern = "pattern";
        size_t pattern_size = 7;
        mu_assert_string_eq(text + 10, simd_strstr(text, text_size, pattern, pattern_size));
}

MU_TEST(test_pattern_not_in_text) {
        const char *text = "This is a text without the Pattern";
        size_t text_size = 34;
        const char *pattern = "pattern";
        size_t pattern_size = 7;
        mu_check(simd_strstr(text, text_size, pattern, pattern_size) == NULL);
}

MU_TEST(test_large_corpus) {
        const char *text = "This is a test of a large corpus that includes a pattern more than once. "
                           "The pattern we are looking for is 'test'. Here it is again: test.";
        size_t text_size = 134;
        const char *pattern = "test";
        size_t pattern_size = 4;
        mu_assert_string_eq(text + 10, simd_strstr(text, text_size, pattern, pattern_size));
}

MU_TEST_SUITE(test_suite) {
        MU_RUN_TEST(test_empty_text);
        MU_RUN_TEST(test_empty_pattern);
        MU_RUN_TEST(test_null_text);
        MU_RUN_TEST(test_null_pattern);
        MU_RUN_TEST(test_pattern_larger_than_text);
        MU_RUN_TEST(test_pattern_at_end_of_text);
        MU_RUN_TEST(test_pattern_at_start_of_text);
        MU_RUN_TEST(test_pattern_in_the_middle_of_text);
        MU_RUN_TEST(test_pattern_not_in_text);
        MU_RUN_TEST(test_large_corpus);
}

int main(int argc, char *argv[]) {
        MU_RUN_SUITE(test_suite);
        MU_REPORT();
        return minunit_status;
}
