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

#include "libebt_util.hh"
#include "test_framework.hh"
#include "test_runner.hh"
#include <string>

using namespace test_framework;
using namespace libebt;

/**
 * \file
 * Test cases for libebt_util.hh.
 */

namespace test_cases
{
    /** \test
     * Test stringify on int.
     */
    struct StringifyIntTests : TestCase
    {
        StringifyIntTests() : TestCase("stringify int") { }

        void run()
        {
            TEST_CHECK_EQUAL(stringify(0),     "0");
            TEST_CHECK_EQUAL(stringify(1),     "1");
            TEST_CHECK_EQUAL(stringify(99),    "99");
            TEST_CHECK_EQUAL(stringify(-99),   "-99");
            TEST_CHECK_EQUAL(stringify(12345), "12345");
        }
    } stringify_int_tests;

    /** \test
     * Test stringify on char *.
     */
    struct StringifyCharStarTests : TestCase
    {
        StringifyCharStarTests() : TestCase("stringify char *") { }

        void run()
        {
            TEST_CHECK_EQUAL(stringify("moo"), std::string("moo"));
            TEST_CHECK_EQUAL(stringify(""), std::string(""));
            TEST_CHECK(stringify("").empty());
            TEST_CHECK_EQUAL(stringify("  quack quack  "), std::string("  quack quack  "));
        }
    } stringify_char_tests;

    /** \test
     * Test stringify on std::string.
     */
    struct StringifyStringTests : TestCase
    {
        StringifyStringTests() : TestCase("stringify string") { }

        void run()
        {
            TEST_CHECK_EQUAL(stringify(std::string("moo")), std::string("moo"));
            TEST_CHECK_EQUAL(stringify(std::string("")), std::string(""));
            TEST_CHECK(stringify(std::string("")).empty());
            TEST_CHECK_EQUAL(stringify(std::string("  quack quack  ")), std::string("  quack quack  "));
        }
    } stringify_string_tests;
}

