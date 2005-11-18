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

dnl output
AM_CONFIG_HEADER(config.h)
AC_OUTPUT(
	Makefile
	libebt/Makefile
	doc/Makefile
	doc/doxygen.conf
	)
