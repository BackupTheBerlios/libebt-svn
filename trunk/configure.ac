dnl vim: set sw=8 sts=8 noet :

AC_INIT([libebt])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([libebt/libebt.hh])
AC_CONFIG_AUX_DIR(config)
AC_SUBST([VERSION_MAJOR])
AC_SUBST([VERSION_MINOR])
AC_SUBST([VERSION_MICRO])
AC_SUBST([VERSION_FULL])

dnl version
VERSION_MAJOR=0
VERSION_MINOR=5
VERSION_MICRO=0
VERSION_FULL="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_MICRO"
VERSION="$VERSION_FULL"

AM_INIT_AUTOMAKE(1.9, [$VERSION_FULL])

dnl check for required programs
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_MAKE_SET

dnl check for cxxflags
LIBEBT_CHECK_CXXFLAG([-W])
LIBEBT_CHECK_CXXFLAG([-Wall])
LIBEBT_CHECK_CXXFLAG([-pedantic])

dnl generated file tag
GENERATED_FILE=m4/generated-file.txt
AC_SUBST_FILE(GENERATED_FILE)

dnl output
AM_CONFIG_HEADER(config.h)
AC_OUTPUT(
	Makefile
	libebt/Makefile
	libebt/libebt_version.hh
	doc/Makefile
	doc/doxygen.conf
	)
