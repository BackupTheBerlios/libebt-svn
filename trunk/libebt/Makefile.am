CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in libebt_version.hh
AM_CXXFLAGS = -I$(top_srcdir)
EXTRA_DIST = libebt_version.hh.in
libincludedir = $(includedir)/libebt
libinclude_HEADERS = libebt.hh libebt_util.hh libebt_version.hh \
	libebt_context.hh libebt_backtracable.hh libebt_backtraceable.hh \
	libebt_boost_threads.hh libebt_zthread_threads.hh libebt_pthread_threads.hh \
	libebt_order.hh libebt_extra_role.hh

the_tests = libebt_context_TEST libebt_backtraceable_TEST libebt_util_TEST \
	libebt_version_TEST
test_headers = test_framework.hh test_runner.hh

if USE_WSTRING_TESTS
the_tests += libebt_wstring_TEST

libebt_wstring_TEST_SOURCES = libebt_wstring_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_wstring_TEST_LDADD = libebttests.a
endif

if USE_BOOST_THREAD_TESTS
the_tests += libebt_boost_threads_TEST

libebt_boost_threads_TEST_SOURCES = libebt_boost_threads_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_boost_threads_TEST_LDADD = libebttests.a
libebt_boost_threads_TEST_LDFLAGS = -lboost_thread
libebt_boost_threads_TEST_CXXFLAGS = -pthread $(AM_CXXFLAGS)
endif

if USE_ZTHREAD_TESTS
the_tests += libebt_zthread_threads_TEST

libebt_zthread_threads_TEST_SOURCES = libebt_zthread_threads_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_zthread_threads_TEST_LDADD = libebttests.a
libebt_zthread_threads_TEST_LDFLAGS = -lZThread
endif

if USE_PTHREAD_TESTS
the_tests += libebt_pthread_threads_TEST

libebt_pthread_threads_TEST_SOURCES = libebt_pthread_threads_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_pthread_threads_TEST_LDADD = libebttests.a
libebt_pthread_threads_TEST_LDFLAGS = -lpthread
endif

TESTS = $(the_tests)

libebttests_a_SOURCES = test_framework.cc test_runner.cc $(test_headers)

libebt_context_TEST_SOURCES = libebt_context_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_context_TEST_LDADD = libebttests.a

libebt_version_TEST_SOURCES = libebt_version_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_version_TEST_LDADD = libebttests.a

libebt_backtraceable_TEST_SOURCES = libebt_backtraceable_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_backtraceable_TEST_LDADD = libebttests.a

libebt_util_TEST_SOURCES = libebt_util_TEST.cc $(test_headers) $(libinclude_HEADERS)
libebt_util_TEST_LDADD = libebttests.a

noinst_PROGRAMS = $(TESTS) libebt_demo
noinst_LIBRARIES = libebttests.a

libebt_demo_SOURCES = libebt_demo.cc $(libinclude_HEADERS)

