dnl vim: set sw=8 sts=8 noet :

AC_INIT([libebt], [0.4.2])
AC_PREREQ(2.5)
AC_CONFIG_SRCDIR([libebt/libebt.hh])
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(1.9)

dnl check for required programs
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_MAKE_SET

dnl cxxflags checks. note that -pedantic is supported but won't produce a
dnl usable compiler on some setups, eg g++ with STLport, so we need to check
dnl whether we can build code which uses the standard library.
AC_DEFUN([LIBEBT_CHECK_CXXFLAG], [
	  libebt_save_CXXFLAGS=$CXXFLAGS
	  CXXFLAGS="$CXXFLAGS $1"
	  AC_MSG_CHECKING([for usable $1 flag])
	  AC_COMPILE_IFELSE([
#include <string>
std::string s;
	],
	[libebt_cxxflag_success=yes],
	[libebt_cxxflag_success=no])
	AC_MSG_RESULT($libebt_cxxflag_success)
	CXXFLAGS="$libebt_save_CXXFLAGS"
	if test "x$libebt_cxxflag_success" = "xyes" ; then
		CXXFLAGS="$CXXFLAGS $1"
	fi
	])
LIBEBT_CHECK_CXXFLAG([-W])
LIBEBT_CHECK_CXXFLAG([-Wall])
LIBEBT_CHECK_CXXFLAG([-pedantic])
LIBEBT_CHECK_CXXFLAG([-Weffc++])

dnl output
AM_CONFIG_HEADER(config.h)
AC_OUTPUT(
	Makefile
	libebt/Makefile
	doc/Makefile
	doc/doxygen.conf
	)
