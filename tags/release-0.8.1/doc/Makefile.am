CLEANFILES = *~ html/* html
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = doxygen.conf.in header.html footer.html doxygen.css libebt_doc.hh

doxygen : doxygen.conf $(top_srcdir)/libebt/*.hh $(top_srcdir)/libebt/*.cc \
		header.html footer.html doxygen.conf libebt_doc.hh
	doxygen doxygen.conf


