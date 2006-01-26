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

#include <pthread.h>
#include <unistd.h>

#include "libebt.hh"
#include "libebt_pthread_threads.hh"
#include "test_framework.hh"
#include "test_runner.hh"

using namespace test_framework;
using namespace libebt;


/**
 * \file
 * Test cases for libebt.hh that make use of pthread.
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
        PthreadBacktraceContextHolder<test_cases::ThreadedExceptionTag>
    {
    };
}
#endif

#include <iostream>

namespace test_cases
{
    /**
     * \test Thread test.
     */
    struct PthreadThreadedTest : TestCase
    {
        PthreadThreadedTest() : TestCase("pthread threaded test (will take up to 30s to run!)") { }

#ifndef DOXYGEN
        static void * run_thread(void * x)
        {
            int i(*reinterpret_cast<int *>(x));
            if (BC::backtrace() != "")
                throw "oops";
            BC c("moo " + stringify(i));
            if (BC::backtrace() != "moo " + stringify(i) + "\n")
                throw "oops";

            sleep(i);

            if (BC::backtrace() != "moo " + stringify(i) + "\n")
                throw "oops";

            pthread_exit(0);
        }
#endif

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            BC c("foo bar");
            TEST_CHECK_EQUAL(BC::backtrace(), "foo bar\n");

            const int thread_count = 10;
            pthread_t threads[thread_count];
            int thread_idxs[thread_count];
            for (int i = 0 ; i < thread_count ; ++i)
            {
                thread_idxs[i] = i;
                int c = pthread_create(&threads[i], 0,
                        &PthreadThreadedTest::run_thread,
                        reinterpret_cast<void *>(&thread_idxs[i]));
                TEST_CHECK_EQUAL(c, 0);
                TEST_CHECK_EQUAL(BC::backtrace(), "foo bar\n");
            }

            int status;
            for (int i = 0 ; i < thread_count ; ++i)
                pthread_join(threads[i], reinterpret_cast<void **>(&status));

            TEST_CHECK_EQUAL(BC::backtrace(), "foo bar\n");

        }
    } pthread_threaded_test;
}

