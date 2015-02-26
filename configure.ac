AC_PREREQ([2.69])
AC_INIT([seisconvert], [0.1.0], [https://github.com/KUM-Kiel/seisconvert])
AM_INIT_AUTOMAKE([foreign])
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([src/kumy2seed.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset mkdir pow strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/bcd/Makefile
                 src/cli/Makefile
                 src/options/Makefile
                 src/seisconvert/Makefile
                 src/tai/Makefile])
AC_OUTPUT
