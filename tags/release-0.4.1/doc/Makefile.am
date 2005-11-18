CLEANFILES = *~ html/* html
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = doxygen.conf.in

doxygen : doxygen.conf $(top_srcdir)/libebt/*.hh $(top_srcdir)/libebt/*.cc
	doxygen doxygen.conf

