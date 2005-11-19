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
#include <string>
#include <list>
#include <iostream>

using namespace test_framework;
using namespace libebt;

/**
 * \file
 * Test cases for libebt.hh.
 */

namespace test_cases
{

#ifndef DOXYGEN
    struct OurContext { };
    typedef BacktraceContext<OurContext> BC;

    struct OurContext2 { };
    typedef BacktraceContext<OurContext2, std::string> BC2;

    struct OurContext3 { };
    typedef BacktraceContext<OurContext3, std::wstring> BC3;
#endif

#ifdef DOXYGEN
    /**
     * Base class for test cases.
     */
    struct TestCase
    {
        /**
         * Run the test case.
         */
        virtual void run();
    };
#endif

    /**
     * \test Basic tests.
     */
    struct BasicTests : TestCase
    {
        BasicTests() : TestCase("basic tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
        }
    } basic_tests;

    /**
     * \test Context tests.
     */
    struct ContextTests : TestCase
    {
        ContextTests() : TestCase("context tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            {
                BC c1("one");
                TEST_CHECK_EQUAL(BC::backtrace(), "one\n");
                {
                    BC c2("two");
                    TEST_CHECK_EQUAL(BC::backtrace(), "one\ntwo\n");
                    {
                        BC c3("three");
                        TEST_CHECK_EQUAL(BC::backtrace(), "one\ntwo\nthree\n");
                    }
                    TEST_CHECK_EQUAL(BC::backtrace(), "one\ntwo\n");
                }
                TEST_CHECK_EQUAL(BC::backtrace(), "one\n");
            }
            TEST_CHECK_EQUAL(BC::backtrace(), "");
        }
    } context_tests;

    /**
     * \test Context tests with multiple tags.
     */
    struct ManyContextsTests : TestCase
    {
        ManyContextsTests() : TestCase("many contexts tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            TEST_CHECK_EQUAL(BC2::backtrace(), "");
            {
                BC c11("one");
                TEST_CHECK_EQUAL(BC::backtrace(), "one\n");
                TEST_CHECK_EQUAL(BC2::backtrace(), "");

                BC2 c21("oink oink");
                TEST_CHECK_EQUAL(BC::backtrace(), "one\n");
                TEST_CHECK_EQUAL(BC2::backtrace(), "oink oink\n");
            }
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            TEST_CHECK_EQUAL(BC2::backtrace(), "");
        }
    } many_contexts_tests;

    /**
     * \test Tests using different delimiters.
     */
    struct DelimTests : TestCase
    {
        DelimTests() : TestCase("delim tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            TEST_CHECK_EQUAL(BC::backtrace(""), "");
            TEST_CHECK_EQUAL(BC::backtrace(" --- "), "");

            BC c1("one");
            TEST_CHECK_EQUAL(BC::backtrace(), "one\n");
            TEST_CHECK_EQUAL(BC::backtrace(""), "one");
            TEST_CHECK_EQUAL(BC::backtrace(" --- "), "one --- ");

            BC c2("two");
            TEST_CHECK_EQUAL(BC::backtrace(), "one\ntwo\n");
            TEST_CHECK_EQUAL(BC::backtrace(""), "onetwo");
            TEST_CHECK_EQUAL(BC::backtrace(" --- "), "one --- two --- ");
        }
    } delim_tests;

    /**
     * \test Simple change context tests.
     */
    struct SimpleChangeTests : TestCase
    {
        SimpleChangeTests() : TestCase("simple change tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            BC c1("item one");
            TEST_CHECK_EQUAL(BC::backtrace(), "item one\n");
            c1.change_context("item one part two");
            TEST_CHECK_EQUAL(BC::backtrace(), "item one part two\n");
        }
    } simple_change_tests;

    /**
     * \test Nested change context tests.
     */
    struct NestedChangeTests : TestCase
    {
        NestedChangeTests() : TestCase("nested change tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            BC c1("hail Eris!");
            TEST_CHECK_EQUAL(BC::backtrace(), "hail Eris!\n");
            {
                BC c2("Fnord");
                TEST_CHECK_EQUAL(BC::backtrace(), "hail Eris!\nFnord\n");
                c2.change_context("Kallisti");
                TEST_CHECK_EQUAL(BC::backtrace(), "hail Eris!\nKallisti\n");
                c1.change_context("All hail Discordia!");
                TEST_CHECK_EQUAL(BC::backtrace(), "All hail Discordia!\nKallisti\n");
            }
            TEST_CHECK_EQUAL(BC::backtrace(), "All hail Discordia!\n");
        }
    } nested_change_tests;

#ifndef DOXYGEN
    class E : public Backtracable<OurContext, std::string>,
              public std::exception
    {
        public:
            E() throw () :
                Backtracable<OurContext, std::string>(),
                std::exception()
            {
            }
    };
#endif

    /**
     * \test Exception tests.
     */
    struct ExceptionTests : TestCase
    {
        ExceptionTests() : TestCase("exception tests") { }

        void run()
        {
            TEST_CHECK_EQUAL(BC::backtrace(), "");
            try
            {
                BC c1("hail");
                BC c2("Eris");
                throw E();
                TEST_CHECK(false);
            }
            catch (const E & e)
            {
                TEST_CHECK_EQUAL(e.backtrace(), "hail\nEris\n");
                TEST_CHECK_EQUAL(BC::backtrace(), "");
            }
        }
    } exception_tests;

    /**
     * \test Wide string tests.
     */
    struct WideStringTests : TestCase
    {
        WideStringTests() : TestCase("wide string tests") { }

        void run()
        {
            TEST_CHECK(BC3::backtrace() == std::wstring(L""));
            BC3 c1(L"wide string");
            TEST_CHECK(BC3::backtrace() == std::wstring(L"wide string\n"));
            BC3 c2(L"wide string two");
            TEST_CHECK(BC3::backtrace() == std::wstring(L"wide string\nwide string two\n"));
        }
    } wide_string_tests;

}

