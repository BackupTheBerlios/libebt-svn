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
#include "test_framework.hh"
#include "test_runner.hh"

#include <memory>
#include <iostream>
#include <zthread/ThreadLocal.h>
#include <zthread/Task.h>
#include <zthread/Thread.h>

using namespace libebt;
using namespace test_framework;

/**
 * \file
 * Test cases for libebt.hh that make use of ZThread.
 */

#ifndef DOXYGEN
namespace test_cases
{
    struct ThreadedExceptionTag { };
    typedef BacktraceContext<ThreadedExceptionTag> BC;
}

template <typename T_>
class RefCountedPtr
{
    private:
        struct Data
        {
            T_ * value;
            unsigned count;
        };

        Data * _data;

    public:
        RefCountedPtr(T_ * const value) :
            _data(new Data)
        {
            _data->value = value;
            _data->count = 1;
        }

        RefCountedPtr(const RefCountedPtr & other) :
            _data(other._data)
        {
            ++(_data->count);
        }

        ~RefCountedPtr()
        {
            if (0 == --(_data->count))
            {
                delete _data->value;
                delete _data;
            }
        }

        T_ & operator* ()
        {
            return *(_data->value);
        }

        T_ * operator-> ()
        {
            return _data->value;
        }
};

namespace libebt
{
    template<>
    struct BacktraceContextHolder<test_cases::ThreadedExceptionTag>
    {
        typedef std::list<std::string> ListType;
        typedef RefCountedPtr<ListType> ListPtrType;

        struct MakeListPtr
        {
            ListPtrType operator() () const
            {
                return ListPtrType(new ListType);
            }
        };

        static ListPtrType get_list()
        {
            static ZThread::ThreadLocal<ListPtrType, MakeListPtr> the_list_ptr;
            return the_list_ptr.get();
        }
    };
}
#endif

namespace test_cases
{
#ifndef DOXYGEN
    class ThreadTask : public ZThread::Runnable
    {
        private:
            int _i;
            bool * const _failed;

        public:
            ThreadTask(const int i, bool * const failed) :
                _i(i),
                _failed(failed)
            {
            }

            void func()
            {
                BC c("func " + stringify(_i));
                if (BC::backtrace() != "run " + stringify(_i) + "\nfunc " + stringify(_i) + "\n")
                    throw std::exception();
                ZThread::Thread::sleep((_i + 1) * 1000);
            }

            void run()
            {
                BC c("run " + stringify(_i));
                try
                {
                    if (BC::backtrace() != "run " + stringify(_i) + "\n")
                        throw std::exception();

                    func();

                    if (BC::backtrace() != "run " + stringify(_i) + "\n")
                        throw std::exception();

                    ZThread::Thread::sleep((10 - _i) * 1000);

                    if (BC::backtrace() != "run " + stringify(_i) + "\n")
                        throw std::exception();

                }
                catch (...)
                {
                    *_failed = true;
                }
            }

    };
#endif

    /**
     * \test Thread test.
     */
    struct ZThreadThreadedTest : TestCase
    {
        ZThreadThreadedTest() : TestCase("zthread test (will take up to 30s to run!)") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            BC c("parent");
            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");

            bool fail0(false) ; ZThread::Thread t0(new ThreadTask(0, &fail0));
            bool fail1(false) ; ZThread::Thread t1(new ThreadTask(1, &fail1));
            bool fail2(false) ; ZThread::Thread t2(new ThreadTask(2, &fail2));
            bool fail3(false) ; ZThread::Thread t3(new ThreadTask(3, &fail3));
            bool fail4(false) ; ZThread::Thread t4(new ThreadTask(4, &fail4));
            bool fail5(false) ; ZThread::Thread t5(new ThreadTask(5, &fail5));
            bool fail6(false) ; ZThread::Thread t6(new ThreadTask(6, &fail6));
            bool fail7(false) ; ZThread::Thread t7(new ThreadTask(7, &fail7));
            bool fail8(false) ; ZThread::Thread t8(new ThreadTask(8, &fail8));
            bool fail9(false) ; ZThread::Thread t9(new ThreadTask(9, &fail9));

            t9.wait();
            t8.wait();
            t7.wait();
            t6.wait();
            t5.wait();
            t4.wait();
            t3.wait();
            t2.wait();
            t1.wait();
            t0.wait();

            TEST_CHECK(! fail0);
            TEST_CHECK(! fail1);
            TEST_CHECK(! fail2);
            TEST_CHECK(! fail3);
            TEST_CHECK(! fail4);
            TEST_CHECK(! fail5);
            TEST_CHECK(! fail6);
            TEST_CHECK(! fail7);
            TEST_CHECK(! fail8);
            TEST_CHECK(! fail9);

            TEST_CHECK_EQUAL(BC::backtrace(), "parent\n");
        }
    } threaded_test;
}


