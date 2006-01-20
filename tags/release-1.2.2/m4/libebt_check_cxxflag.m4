AC_DEFUN([LIBEBT_CHECK_CXXFLAG], [
	  libebt_save_CXXFLAGS=$CXXFLAGS
	  CXXFLAGS="$CXXFLAGS $1"
	  AC_MSG_CHECKING([for usable $1 flag])
	  AC_COMPILE_IFELSE([
#include <string>
#include <iostream>
void f()
{
	std::string s("test");
	std::cout << s << std::endl;
}
	],
	[libebt_cxxflag_success=yes],
	[libebt_cxxflag_success=no])
	AC_MSG_RESULT($libebt_cxxflag_success)
	CXXFLAGS="$libebt_save_CXXFLAGS"
	if test "x$libebt_cxxflag_success" = "xyes" ; then
		CXXFLAGS="$CXXFLAGS $1"
	fi
	])

