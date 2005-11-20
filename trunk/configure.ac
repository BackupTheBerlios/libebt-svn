dnl vim: set sw=8 sts=8 noet :

AC_PREREQ(2.59)
AC_INIT([libebt/libebt.hh])
AC_CONFIG_AUX_DIR(config)

VERSION_MAJOR=0
VERSION_MINOR=6
VERSION_MICRO=1
VERSION_FULL="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_MICRO"
VERSION="$VERSION_FULL"

AC_SUBST([VERSION_MAJOR])
AC_SUBST([VERSION_MINOR])
AC_SUBST([VERSION_MICRO])
AC_SUBST([VERSION_FULL])

AM_INIT_AUTOMAKE(libebt, [$VERSION_FULL])

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

dnl check whether we should do wstring
AC_MSG_CHECKING([whether we should do std::wstring tests])
AC_COMPILE_IFELSE([
#include <string>
std::wstring s;
	],
	[use_wstring=yes],
	[use_wstring=no])
AC_MSG_RESULT($use_wstring)
AM_CONDITIONAL(USE_WSTRING_TESTS, test x"$use_wstring" = "xyes")

dnl check whether we should do thread tests
save_CXXFLAGS=$CXXFLAGS
CXXFLAGS="$CXXFLAGS -pthread"
AC_MSG_CHECKING([whether we should do boost::thread tests])
AC_COMPILE_IFELSE([
#include <boost/thread.hpp>
#include <memory>
void f()
{
}

void g()
{
    std::auto_ptr<boost::thread> t(new boost::thread(f));
}
	],
	[use_thread=yes],
	[use_thread=no])
CXXFLAGS=$save_CXXFLAGS
AC_MSG_RESULT($use_thread)
AM_CONDITIONAL(USE_THREAD_TESTS, test x"$use_thread" = "xyes")

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
