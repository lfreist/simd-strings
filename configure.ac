# configure.ac
AC_INIT([project_name], [1.0], [your_email@example.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AM_PROG_AR
LT_INIT
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

src_files=$(find src -name "*.c" -printf "%p ")

AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

