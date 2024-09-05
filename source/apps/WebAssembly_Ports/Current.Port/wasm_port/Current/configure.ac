#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT(current, 1.0, jared@lostsidedead.biz)
AM_INIT_AUTOMAKE(current, 1.0)
# Checks for programs.
AC_PROG_CXX
AC_LANG_CPLUSPLUS

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

LDPREFIX=${prefix}

if test  ${prefix} = "NONE"; then
LDPREFIX=/usr/local
fi

MX_CFLAGS="-std=c++98 -I$LDPREFIX/include/mx -I$LDPREFIX/include `sdl-config --cflags`"
MX_LDFLAGS="`sdl-config --libs ` -L$LDPREFIX/lib -lmx"

echo $MX_CFLAGS
echo $MX_LDFLAGS 

AC_SUBST(MX_CFLAGS)
AC_SUBST(MX_LDFLAGS)

AC_OUTPUT(Makefile source/Makefile)
