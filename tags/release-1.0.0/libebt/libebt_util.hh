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

/// Include guard.
#ifndef HAVE_THE_LIBEBT_LIBEBT_UTIL_HH
#define HAVE_THE_LIBEBT_LIBEBT_UTIL_HH 1

#include <string>
#include <sstream>

/** \file
 * Utility functions.
 */

namespace libebt
{
    /**
     * Utility function which creates a string containing a newline.
     */
    template <typename StringType_>
    inline StringType_ newline_string()
    {
        return StringType_(1, static_cast<typename StringType_::value_type>('\n'));
    }

    /**
     * Utility function which creates a string containing a newline
     * (specialisation for std::string).
     */
    template <>
    inline std::string newline_string<std::string>()
    {
        return "\n";
    }

    /**
     * Convert item to a string.
     */
    template <typename T_>
    std::string
    stringify(const T_ & item)
    {
        std::ostringstream s;
        s << item;
        return s.str();
    }

    /**
     * Convert item to a string (specialisation for std::string).
     */
    template <>
    inline std::string
    stringify<std::string>(const std::string & item)
    {
        return item;
    }
}

#endif
