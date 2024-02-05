from setuptools import setup, Extension

setup(
    name="simd_strings",
    version="0.0.1",
    ext_modules=[Extension("simd_string", ["simd_strings.c"])],
    description="Python bindings to simd_strings",
    author="Leon Freist",
    author_email="freist.leon@gmail.com",
    url="https://github.com/lfreist/simd_strings"
)