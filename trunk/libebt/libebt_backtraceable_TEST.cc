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
 * Test cases for libebt_backtraceable.hh related to the
 * libebt::Backtraceable class template.
 */

namespace test_cases
{

#ifndef DOXYGEN
    struct OurContext { };
    typedef BacktraceContext<OurContext> BC;

    struct OurContext2 { };
    typedef BacktraceContext<OurContext2, std::string> BC2;
#endif

#ifndef DOXYGEN
    class E : public Backtraceable<OurContext, std::string>,
              public std::exception
    {
        public:
            E() throw () :
                Backtraceable<OurContext, std::string>(),
                std::exception()
            {
            }

            template <typename T_>
            E(const Backtraceable<T_, std::string> & other,
                    const std::string & extra,
                    const ExtraRole role) :
                Backtraceable<OurContext, std::string>(other, extra, role),
                std::exception()
            {
            }
    };

    class E2 : public Backtraceable<OurContext2, std::string>,
               public std::exception
    {
        public:
            E2() throw () :
                Backtraceable<OurContext2, std::string>(),
                std::exception()
            {
            }

            template <typename T_>
            E2(const Backtraceable<T_, std::string> & other,
                    const std::string & extra,
                    const ExtraRole role) :
                Backtraceable<OurContext2, std::string>(other, extra, role),
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
                throw E();
            }
            catch (const E & e)
            {
                TEST_CHECK(e.backtrace().empty());
            }

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
                TEST_CHECK_EQUAL(e.backtrace("~"), "hail~Eris~");

                TEST_CHECK_EQUAL(e.backtrace("\n", oldest_first), "hail\nEris\n");
                TEST_CHECK_EQUAL(e.backtrace("~", oldest_first), "hail~Eris~");

                TEST_CHECK_EQUAL(e.backtrace("\n", newest_first), "Eris\nhail\n");
                TEST_CHECK_EQUAL(e.backtrace("~", newest_first), "Eris~hail~");

                TEST_CHECK(! e.backtrace().empty());
                TEST_CHECK_EQUAL(BC::backtrace(), "");
            }
        }
    } exception_tests;

    /**
     * \test Merge tests.
     */
    struct MergeTests : TestCase
    {
        MergeTests() : TestCase("merge tests") { }

        void run()
        {
            for (ExtraRole role = joiner_item ; role <= suffix_each_item ;
                    role = static_cast<ExtraRole>(static_cast<int>(role) + 1))
            {
                TEST_CHECK_EQUAL(BC::backtrace(), "");
                TEST_CHECK_EQUAL(BC2::backtrace(), "");

                try
                {
                    BC c1("one");
                    BC c2("two");
                    try
                    {
                        BC2 c21("three");
                        BC2 c22("four");
                        throw E2();

                        TEST_CHECK(false);
                    }
                    catch (E2 & e2)
                    {
                        throw E(e2, "(x)", role);
                    }
                    TEST_CHECK(false);
                }
                catch (E & e)
                {
                    switch (role)
                    {
                        case joiner_item:
                            TEST_CHECK_EQUAL(e.backtrace("."), "one.two.(x).three.four.");
                            break;

                        case prefix_each_item:
                            TEST_CHECK_EQUAL(e.backtrace("."), "one.two.(x)three.(x)four.");
                            break;

                        case suffix_each_item:
                            TEST_CHECK_EQUAL(e.backtrace("."), "one.two.three(x).four(x).");
                            break;

                        default:
                            TEST_CHECK(false);
                    }
                }
            }
        }
    } test_merge;
}


