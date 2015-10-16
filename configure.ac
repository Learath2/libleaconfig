#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libleaconfig], [0.0.0])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([libleaconfig/leaconfig.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_PREREQ([2.4])
AM_PROG_AR
LT_INIT

# Checks for programs.
AM_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([limits.h stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([memmove memset strchr strstr strtol])


AC_CONFIG_FILES([Makefile
		 include/Makefile
		 include/libleaconfig/Makefile
		 libleaconfig/Makefile
                 src/Makefile])
AC_OUTPUT
echo \
"-------------------------------------------------

 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}

 Prefix: '${prefix}'.
 Compiler: '${CC} ${CFLAGS}'
 Libraries: '${LIBS}'

--------------------------------------------------"
