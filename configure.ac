# Process this file with autoconf to produce a configure script.

PATH=$PATH:/usr/bin:/bin:/usr/local/bin:/usr/ccs/bin:/usr/contrib/bin:/usr/ucb/bin
export PATH

AC_INIT([Resurrection], [0.0.0])
AC_PREREQ(2.50)

R_SAVE_OPTS([$@])

R_SET_CC_FLAGS

AC_CONFIG_AUX_DIR(config_aux)

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

R_SET_PATHS

AC_PROG_CC([gcc cl cc clang])
AM_PROG_CC_C_O
AM_PROG_LIBTOOL

R_SET_TEMPLATES

#R_ENABLE_GPROF

R_CHECK_HEADERS
R_CHECK_SIZES
R_CHECK_TYPES
R_CHECK_LIBS
R_CHECK_MMAP_CORE
R_CHECK_SBRK_KLUDGE

AC_CONFIG_HEADERS(include/Resurrection/autoconf.h)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(include/Makefile)
AC_CONFIG_FILES(lib/Makefile)
AC_CONFIG_FILES(lib/Resurrection/Makefile)
AC_CONFIG_FILES(lib/Rwm/Makefile)
AC_CONFIG_FILES(lib/Rterm/Makefile)
AC_CONFIG_FILES(lib/Rv/Makefile)
AC_CONFIG_FILES(lib/Rl/Makefile)
AC_CONFIG_FILES(apps/Makefile)
AC_CONFIG_FILES(apps/Rwm/Makefile)
AC_CONFIG_FILES(apps/Rterm/Makefile)
AC_CONFIG_FILES(apps/Rv/Makefile)
AC_CONFIG_FILES(apps/Rl/Makefile)
AC_CONFIG_FILES(apps/Rsaver/Makefile)
AC_CONFIG_FILES(images/Makefile)
AC_CONFIG_FILES(fonts/Makefile)
AC_CONFIG_FILES(test/Makefile)

AC_OUTPUT
R_PRINT_INFO

