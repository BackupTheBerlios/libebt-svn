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

#include "test_framework.hh"
#include <iostream>
#include <algorithm>

using namespace test_framework;

struct TestCase::Impl
{
    const std::string name;

    Impl(const std::string & the_name) :
        name(the_name)
    {
    }
};

TestCase::TestCase(const std::string & name) :
    _impl(new Impl(name))
{
    TestCaseList::register_test_case(this);
}

TestCase::~TestCase()
{
}

void
TestCase::check(const char * const function, const char * const file,
        const long line, bool was_ok, const std::string & message) const
{
    std::cout << "." << std::flush;
    if (! was_ok)
        throw TestFailedException(function, file, line, message);
}

void
TestCase::call_run()
{
    try
    {
        run();
    }
    catch (TestFailedException)
    {
        throw;
    }
    catch (std::exception &e)
    {
        throw TestFailedException(__PRETTY_FUNCTION__, __FILE__, __LINE__,
                "Test threw unexpected exception " + libebt::stringify(e.what()));
    }
    catch (...)
    {
        throw TestFailedException(__PRETTY_FUNCTION__, __FILE__, __LINE__,
                "Test threw unexpected unknown exception");
    }
}

std::string
TestCase::name() const
{
    return _impl->name;
}

TestFailedException::TestFailedException(const char * const function, const char * const file,
        const long line, const std::string & message) throw () :
    _message(message + " (in " + function + " at " + file + " line " + libebt::stringify(line) + ")")
{
}

TestFailedException::~TestFailedException() throw ()
{
}

TestCaseList::TestCaseList()
{
}

TestCaseList::~TestCaseList()
{
}

class RunTest
{
    private:
        bool * const _had_a_failure;

    public:
        RunTest(bool * had_a_failure) :
            _had_a_failure(had_a_failure)
        {
        }

        void operator() (TestCase * test_case) const
        {
            bool had_local_failure(false);

            std::cout << "* \"" << test_case->name() << "\": " << std::flush;

            try
            {
                test_case->call_run();
            }
            catch (std::exception &e)
            {
                std::cout << "!{" << e.what() << "} " << std::flush;
                had_local_failure = true;
                *_had_a_failure = true;
            }
            catch (...)
            {
                std::cout << "!{Unknown exception type} ";
                had_local_failure = true;
                *_had_a_failure = true;
            }

            if (had_local_failure)
                std::cout << " NOT OK" << std::endl;
            else
                std::cout << " OK" << std::endl;
        }
};

bool
TestCaseList::run_tests()
{
    bool had_a_failure(false);

    std::for_each(
            TestCaseList::_get_test_case_list().begin(),
            TestCaseList::_get_test_case_list().end(),
            RunTest(&had_a_failure));

    return ! had_a_failure;
}


