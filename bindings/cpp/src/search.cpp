/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of simd_string.
 */

#include <simdstr/search.hpp>

extern "C" {
#include <simdstr/search.h>
};

namespace simdstr {

const char* strstr(const char* haystack, size_t haystack_size, const char* needle, size_t needle_size) {
  return simd_strstr(haystack, haystack_size, needle, needle_size);
}

const char* strstr(const std::string& haystack, const std::string& needle) {
  return strstr(haystack.data(), haystack.size(), needle.data(), needle.size());
}

const char* strstr(std::string_view haystack, std::string_view needle) {
  return strstr(haystack.data(), haystack.size(), needle.data(), needle.size());
}

}  // namespace simdstr