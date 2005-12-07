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
 * Test cases for libebt_backtracable.hh related to the
 * libebt::Backtracable class template.
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
                TEST_CHECK_EQUAL(e.backtrace("~"), "hail~Eris~");
                TEST_CHECK_EQUAL(BC::backtrace(), "");
            }
        }
    } exception_tests;
}


