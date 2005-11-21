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

using namespace test_framework;
using namespace libebt;

/**
 * \file
 * Test cases for libebt_version.hh .
 */

namespace test_cases
{
    /**
     * \test Version tests.
     */
    struct VersionTest : TestCase
    {
        VersionTest() : TestCase("version test") { }

        void run()
        {
            TEST_CHECK(LIBEBT_VERSION_MAJOR >= 0);
            TEST_CHECK(LIBEBT_VERSION_MAJOR <= 9);

            TEST_CHECK(LIBEBT_VERSION_MINOR >= 0);
            TEST_CHECK(LIBEBT_VERSION_MINOR <= 99);

            TEST_CHECK(LIBEBT_VERSION_MICRO >= 0);
            TEST_CHECK(LIBEBT_VERSION_MICRO <= 99);

            TEST_CHECK(LIBEBT_VERSION >= 0);
            TEST_CHECK(LIBEBT_VERSION <= 99999);
            TEST_CHECK_EQUAL(LIBEBT_VERSION, 10000 * LIBEBT_VERSION_MAJOR +
                    100 * LIBEBT_VERSION_MINOR + LIBEBT_VERSION_MICRO);
        }
    } version_test;
}
