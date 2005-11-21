/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2005 Ciaran McCreesh
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the library nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libebt.hh"
#include "libebt/libebt_boost_threads.hh"
#include "test_framework.hh"
#include "test_runner.hh"

#include <memory>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/lambda/bind.hpp>

using namespace libebt;
using namespace test_framework;

/**
 * \file
 * Test cases for libebt.hh that make use of boost.threads.
 */

#ifndef DOXYGEN
namespace test_cases
{
    struct ThreadedExceptionTag { };
    typedef BacktraceContext<ThreadedExceptionTag> BC;
}

namespace libebt
{
    template<>
    struct BacktraceContextHolder<test_cases::ThreadedExceptionTag> :
        BoostThreadsBacktraceContextHolder<test_cases::ThreadedExceptionTag>
    {
    };
}
#endif

namespace test_cases
{
    /**
     * \test Thread test.
     */
    struct BoostThreadedTest : TestCase
    {
        BoostThreadedTest() : TestCase("boost threaded test (will take up to 30s to run!)") { }

#ifndef DOXYGEN
        void func2(int i)
        {
            BC c("func2 " + stringify(i));
            TEST_CHECK_EQUAL(BC::backtrace(), "func1 " + stringify(i) + "\nfunc2 "
                    + stringify(i) + "\n");

            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.sec += (i + 1);
            boost::thread::sleep(xt);
        }

        void func1(int i, bool * const failed)
        {
            try
            {
                BC c("func1 " + stringify(i));
                TEST_CHECK_EQUAL(BC::backtrace(), "func1 " + stringify(i) + "\n");
                func2(i);
                TEST_CHECK_EQUAL(BC::backtrace(), "func1 " + stringify(i) + "\n");

                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.sec += (10 - i);
                boost::thread::sleep(xt);

                TEST_CHECK_EQUAL(BC::backtrace(), "func1 " + stringify(i) + "\n");
            }
            catch (...)
            {
                *failed = true;
            }
        }
#endif

        void run()
        {
            const int thread_count = 10;

            TEST_CHECK_EQUAL(BC::backtrace(), "");
            BC c("parent");
            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");

            std::auto_ptr<boost::thread> threads[thread_count];
            bool failed[thread_count];
            for (int i = 0 ; i < thread_count ; ++i)
            {
                failed[i] = false;
                threads[i] = std::auto_ptr<boost::thread>(new boost::thread(
                            boost::lambda::bind(&BoostThreadedTest::func1, this, i, &failed[i])));
            }

            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");

            for (int i = 0 ; i < thread_count ; ++i)
                threads[i]->join();

            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");

            for (int i = 0 ; i < thread_count ; ++i)
                check(__PRETTY_FUNCTION__, __FILE__, __LINE__, ! failed[i],
                        "Thread " + stringify(i) + " failed");

            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");
        }
    } threaded_test;
}

