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

#ifndef HAVE_LIBEBT_TEST_FRAMEWORK_HH
#define HAVE_LIBEBT_TEST_FRAMEWORK_HH 1

#include "config.h"
#include <libebt/libebt_util.hh>
#include <string>
#include <memory>
#include <list>
#include <sstream>

namespace test_framework
{
    class TestCase
    {
        private:
            struct Impl;
            std::auto_ptr<Impl> _impl;

        protected:
            void check(const char * const function, const char * const file,
                    const long line, bool was_ok, const std::string & message) const;

            virtual void run() = 0;

        public:
            TestCase(const std::string & name);
            virtual ~TestCase();

            void call_run();

            std::string name() const;
    };

    class TestFailedException : public std::exception
    {
        private:
            const std::string _message;

        public:
            TestFailedException(const char * const function, const char * const file,
                    const long line, const std::string & message) throw ();
            virtual ~TestFailedException() throw ();

            inline const char * what() const throw ()
            {
                return _message.c_str();
            }
    };

    class TestCaseList
    {
        private:
            TestCaseList();
            ~TestCaseList();

            static std::list<TestCase *> & _get_test_case_list()
            {
                static std::list<TestCase *> l;
                return l;
            }

        public:
            inline static void register_test_case(TestCase * const test_case)
            {
                _get_test_case_list().push_back(test_case);
            }

            static bool run_tests();
    };

}

#define TEST_CHECK_EQUAL(a, b) \
    do { \
        check(__PRETTY_FUNCTION__, __FILE__, __LINE__, a == b, \
                "Expected '" #a "' to equal '" + libebt::stringify(b) + \
                "' but got '" + libebt::stringify(a) + "'"); \
    } while (false)

#define TEST_CHECK(a) \
    do { \
        check(__PRETTY_FUNCTION__, __FILE__, __LINE__, a, \
                "Check '" #a "' failed"); \
    } while (false)

#define TEST_CHECK_THROWS(a, b) \
    do { \
        try { \
            TEST_CHECK(true); \
            a; \
            check(__PRETTY_FUNCTION__, __FILE__, __LINE__, false, \
                    "Expected exception of type '" #b "' not thrown"); \
        } catch (b & e) { \
        } \
    } while (false)

#endif
