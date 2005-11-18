CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in configure config/* aclocal.m4 \
			config.h config.h.in doxygen.conf
AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip
EXTRA_DIST = autogen.bash LICENCE README
SUBDIRS = libebt doc

doxygen :
	$(MAKE) -C doc doxygen

