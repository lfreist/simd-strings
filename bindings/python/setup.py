from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import subprocess


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        build_temp = os.path.join(self.build_temp, 'py_project')
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DCMAKE_BUILD_TEMP=' + build_temp,
                      '-DPYTHON_EXECUTABLE=' + sys.executable,
                      ext.sourcedir]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(['cmake', *cmake_args], cwd=self.build_temp)

        # Set RPATH to include the library directory
        rpath = os.path.join(extdir, '..', 'lib', 'your_project_name')
        subprocess.check_call(['cmake', '--build', '.'], cwd=self.build_temp)


setup(
    name='py_simdstr',
    version='0.0.1',
    author='Leon Freist',
    author_email="freist.leon@gmail.com",
    description='Python bindings for simd_strings',
    ext_modules=[CMakeExtension('py_simdstr', sourcedir='../../')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)
