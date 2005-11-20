/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/** \file
 * Main page documentation for Doxygen.
 */

/** \mainpage
 * \if false
 * You can use doxygen to get this documentation in HTML format.
 * \endif
 *
 * \section overview Overview
 *
 * The libebt library provides a clean way of getting human-readable backtrace
 * messages in C++. It uses the <a
 * href="http://www.research.att.com/~bs/bs_faq2.html#finally">RAII (resource
 * acquisition is initialisation) idiom</a> to manage backtrace stack items
 * without needing macros, control structures or an external debugger.
 *
 * A typical application using libebt will produce errors along the lines of
 * the following:
 * \verbatim
Caught exception:
 * When performing query 'app-editors/vim' from commandline:
-> When calculating dependencies for 'app-editors/vim':
-> When calculating dependencies for 'app-editors/vim-core':
-> When loading versions for 'gentoo::app-editors/vim-core':
-> When parsing version string '6.4_invalid_version_suffix':
Version error: '6.4_invalid_version_suffix' is invalid (unknown format at around character offset 3)
\endverbatim
 *
 * This is far more useful for both end users and developers than a simple
 * context-free error message, and does not require the use of a debugger to
 * generate the backtrace.
 *
 * \section usage Usage
 *
 * Here's a simple preprocessor. It takes the name of a file and copies each
 * non-directive line to stdout. If a line starts with '\#e ', it generates
 * an error message from the rest of the line. If a line starts with '\#i ',
 * it includes the filename specified on the remainder of the line. Any other
 * line starting with a '\#' is an invalid directive.
 *
 * \include libebt_demo.cc
 *
 * This gives backtraces like the following:
 *
 * \verbatim
Error!
  * In main program:
  * When handling commandline argument 'first.txt':
  * In file 'first.txt':
  * From #i directive on line 2:
  * In file 'second.txt':
  * From #i directive on line 5:
  * In file 'third.txt':
  * Error directive on line 2: 'This is an error.' (21FileHasErrorException)
\endverbatim
 *
 * Backtraces are also available for non-exceptions (for example, for log
 * messages or warnings). See the class documentation for details.
 *
 * libebt is a pure template library, so there is no need to link your
 * application against anything. Usually libebt is installed into the
 * standard include path, so no CXXFLAGS are needed either.
 *
 * \section compiling Compiling libebt
 *
 * libebt uses a standard autotools setup. Typically it can be built using:
 *
 * \verbatim
./configure
make
make check
sudo make install
\endverbatim
 *
 * If you have <a href="http://www.doxygen.org/">Doxygen</a>, you can make some
 * HTML documentation:
 *
 * \verbatim
make doxygen
\endverbatim
 *
 * If you are using a Subversion checkout rather than a source tarball, you
 * will probably need to run:
 *
 * \verbatim
./autogen.bash
\endverbatim
 *
 * to create the configure script. This will require a full GNU autotools setup
 * on your system.
 *
 * libebt does not need any libraries beyond a C++ standard library
 * implementation.
 */

/** \page Authors Authors
 * \section Authors
 *
 * libebt was written by Ciaran McCreesh <ciaranm@gentoo.org>.
 */

/** \page Threads Threads \section threads Lack of Thread Safety
 *
 * <b>libebt will almost certainly have problems in threaded environments!</b>
 *
 * It is generally simple enough to adapt libebt to work with any particular
 * thread library through use of a thread-local variable for the static
 * variable in <code>libebt::BacktraceContext::_get_list()</code> . However,
 * there is no standard way of doing this.
 */

/** \page Overhead Overhead
 * \section overhead Overhead
 *
 * libebt's overhead is reasonably low. If performance is a particular
 * concern, avoid specifying exception context inside small functions or
 * tight loops.
 *
 * Note that it is possible to introduce huge amounts of overhead when
 * creating the context string. Avoid putting expensive-to-get information
 * in context strings if possible.
 */

 /** \page Portability Portability
 * \section portability Portability
 *
 * libebt needs a C++ compiler which can handle member templates, and which
 * comes with a reasonable standard library implementation. There is no
 * platform-dependent or architecture-dependent code.
 *
 * libebt has been reported to work with:
 * - gcc 3.3.6 on Linux
 * - gcc 3.3.6 + STLport on Linux
 * - Intel(R) C++ Compiler 7.1 on Linux
 * - gcc 3.4.4 on FreeBSD
 * - gcc 4.1.0 on FreeBSD
 *
 * It has been tested on x86, x86_64, mips and sparc.
 */

/** \page ChangeLog ChangeLog
 * \section ChangeLog ChangeLog
 * \verbinclude ChangeLog
 */

/** \page Licence Licence
 * \section Licence Licence
 * \verbinclude LICENCE
 */

/**
 * \namespace test_cases
 * Test cases.
 */

/** \namespace libebt
 * Holds the classes used by libebt.
 */

