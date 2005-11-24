dnl vim: set sw=8 sts=8 noet :

AC_PREREQ(2.59)
AC_INIT([libebt/libebt.hh])
AC_CONFIG_AUX_DIR(config)

VERSION_MAJOR=0
VERSION_MINOR=8
VERSION_MICRO=2
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
AC_ARG_ENABLE(wstring-tests,
	      [  --enable-wstring-tests      Enable tests using std::wstring.],
	      [use_wstring=$enableval],
	      [
		AC_COMPILE_IFELSE([
#include <string>
std::wstring s;
		],
			[use_wstring=yes],
			[use_wstring=no])])
AC_MSG_RESULT($use_wstring)
AM_CONDITIONAL(USE_WSTRING_TESTS, test x"$use_wstring" = "xyes")

dnl check whether we should do boost thread tests
save_CXXFLAGS=$CXXFLAGS
save_LIBS=$LIBS
CXXFLAGS="$CXXFLAGS -pthread"
LIBS="-lboost_thread $LIBS"
AC_MSG_CHECKING([whether we should do boost.threads tests])
AC_ARG_ENABLE(boost-thread-tests,
	      [  --enable-boost-thread-tests Enable tests using boost.threads.],
	      [use_boost_thread=$enableval],
	      [AC_TRY_LINK([
#include <boost/thread.hpp>
#include <memory>

void f()
{
}
	],[
    std::auto_ptr<boost::thread> t(new boost::thread(&f));
	],
		[use_boost_thread=yes],
		[use_boost_thread=no])])
CXXFLAGS=$save_CXXFLAGS
LIBS=$save_LIBS
AC_MSG_RESULT($use_boost_thread)
AM_CONDITIONAL(USE_BOOST_THREAD_TESTS, test x"$use_boost_thread" = "xyes")

dnl check whether we should do ZThread thread tests
AC_MSG_CHECKING([whether we should do ZThread tests])
AC_ARG_ENABLE(zthread-tests,
	      [  --enable-zthread-tests      Enable tests using ZThread.],
	      [use_zthread=$enableval],
	      [
		AC_COMPILE_IFELSE([
#include <zthread/Thread.h>
class R : public ZThread::Runnable
{
	public:
		void run()
		{
		}
};
	],
		[use_zthread=yes],
		[use_zthread=no])])
AC_MSG_RESULT($use_zthread)
AM_CONDITIONAL(USE_ZTHREAD_TESTS, test x"$use_zthread" = "xyes")

dnl check whether we should do pthread thread tests
AC_MSG_CHECKING([whether we should do pthread tests])
AC_ARG_ENABLE(pthread-tests,
	      [  --enable-pthread-tests      Enable tests using pthread.],
	      [use_pthread=$enableval],
	      [
		AC_COMPILE_IFELSE([
#include <pthread.h>
	],
		[use_pthread=yes],
		[use_pthread=no])])
AC_MSG_RESULT($use_pthread)
AM_CONDITIONAL(USE_PTHREAD_TESTS, test x"$use_pthread" = "xyes")

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
