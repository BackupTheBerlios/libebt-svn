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
 * without needing macros, control structures or an external debugger --
 * adding context to a function or block is done through a single simple
 * variable declaration.
 *
 * A typical application using libebt will produce errors along the lines of
 * the following:
 *
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
 * it includes the filename specified on the remainder of the line. If a line
 * starts with '\#w ', it generates a warning (with context) from the rest of
 * the line. Any other line starting with a '\#' is an invalid directive.
 *
 * Point to note:
 * - To avoid confusion with multiple libraries making use of libebt, a tag
 *   is used to keep track of the context.
 * - Rather than defining a specific exception class, libebt provides a
 *   libebt::Backtracable template class which can be used in conjunction with
 *   other exception heirarchies through multiple inheritance.
 * - Declaring context for a given block or function is as simple as declaring
 *   a variable. To avoid typing out libebt::Backtracable<ExceptionTag> in
 *   several places, a typedef is used.
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
 * Further things possible with libebt include:
 * - Support for other string classes (for example, <code>std::wstring</code>)
 * - Support for using a thread-safe context stack for any thread library
 *   which supports thread-specific storage.
 *
 * See the class documentation for details.
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
 * implementation. If the <a href="http://www.boost.org/">Boost</a> library
 * <code>Boost.Threads</code> is available, it will be used for one of the test
 * cases; however, <b>Boost is not required</b>. Similarly, if the <a
 * href="http://zthread.sourceforge.net/">ZThread</a> library is available, it
 * will be used for a test case.
 *
 * Headers for use with ZThread and Boost will be installed, but they are not
 * used except in test cases or when explicitly \#included.
 */

/** \page Authors Authors
 * \section Authors
 *
 * libebt was written by Ciaran McCreesh <ciaranm@gentoo.org>.
 */

/** \page Threads Threads
 * \section threads Using libebt with Threads
 *
 * By default, <b>libebt will almost certainly have problems in threaded
 * environments</b>! However, it can probably be made to work correctly
 * alongside most thread libraries.
 *
 * Generally, providing a specialisation of libebt::BacktraceContextHolder that
 * uses thread specific storage is all that is needed. If you are using Boost
 * threads or ZThread, an existing helper is already available. Otherwise, have
 * a look at libebt/libebt_boost_threads.hh and libebt/libebt_zthread_threads.hh
 * for how to get started.
 *
 * \subsection boost Boost Threads
 *
 * Here's a brief snippet demonstrating how to use libebt with the Boost
 * thread library. Note that context from the owning thread is <b>not</b>
 * copied into the child thread using this method.
 *
\code
#include <boost/thread.hpp>
#include <libebt/libebt.hh>
#include <libebt/libebt_boost_threads.hh>

struct ExceptionTag { };
typedef BacktraceContext<ExceptionTag> Context;

namespace libebt
{
    template<>
    struct BacktraceContextHolder<ThreadedExceptionTag> :
        BoostThreadsBacktraceContextHolder<ThreadedExceptionTag>
    {
    };
}
\endcode
 *
 * \subsection zthread ZThread Threads
 *
 * Here's an example for ZThread threads. Again, the parent thread context is
 * not copied to the child threads.
 *
\code
#include <boost/thread.hpp>
#include <libebt/libebt.hh>
#include <libebt/libebt_zthread_threads.hh>

struct ExceptionTag { };
typedef BacktraceContext<ExceptionTag> Context;

namespace libebt
{
    template<>
    struct BacktraceContextHolder<ThreadedExceptionTag> :
        ZThreadBacktraceContextHolder<ThreadedExceptionTag>
    {
    };
}
\endcode
 */

/** \page Overhead Overhead
 * \section overhead Overhead
 *
 * libebt's overhead is reasonably low. If performance is of particular
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
 * If the <a href="http://www.boost.org/">Boost</a> library
 * <code>Boost.Threads</code> is available, it will be used for one of the test
 * cases; however, <b>Boost is not required</b>. Similarly, the <a
 * href="http://zthread.sourceforge.net/">ZThread</a> library, if present, will
 * be used for a test case.
 *
 * Helper headers for both Boost and ZThread will be installed, but they are not
 * used unless explicitly \#included.
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

