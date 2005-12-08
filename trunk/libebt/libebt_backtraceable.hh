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
#ifndef HAVE_THE_LIBEBT_LIBEBT_BACKTRACEABLE_HH
#define HAVE_THE_LIBEBT_LIBEBT_BACKTRACEABLE_HH 1

#include <libebt/libebt_util.hh>
#include <libebt/libebt_context.hh>

#include <list>
#include <string>
#include <algorithm>
#include <iterator>

/** \file
 * Definitions for the libebt::Backtraceable class template, and the
 * deprecated libebt::Backtracable alias.
 */
namespace libebt
{
    /**
     * Exception base class which includes backtrace information.
     *
     * The Tag_ template parameter should be an empty struct. It is used for
     * backtrace grouping.
     *
     * The StringType_ template parameter specifies the type of string used for
     * messages.
     */
    template <typename Tag_, typename StringType_ = std::string>
    class Backtraceable
    {
        private:
            /**
             * Our backtrace list type. Internal use only.
             */
            typedef std::list<StringType_> ListType;

            ListType _backtrace;

        protected:
            /**
             * Constructor. Grabs backtrace information from the stack and
             * remembers it.
             */
            Backtraceable() throw () :
                _backtrace()
            {
                BacktraceContext<Tag_, StringType_>::copy_backtrace_items_to(
                        std::back_inserter(_backtrace));
            }

            /**
             * Copy constructor.
             */
            Backtraceable(const Backtraceable & other) throw () :
                _backtrace(other._backtrace.begin(), other._backtrace.end())
            {
            }

            /**
             * Destructor.
             */
            virtual ~Backtraceable() throw ()
            {
            }

        public:
            /**
             * String representation of our backtrace.
             *
             * \param item_terminator A terminator which is appended to every
             *                        context item. Defaults to a newline.
             */
            StringType_ backtrace(const StringType_ & item_terminator =
                    newline_string<StringType_>()) const;

            /**
             * Copy backtrace items to the insert iterator I_.
             */
            template <typename I_>
            void copy_backtrace_items_to(I_ i) const
            {
                std::copy(_backtrace.begin(), _backtrace.end(), i);
            }
    };

    /**
     * Deprecated wrapper for the Backtraceable class, for backwards
     * compatibility.
     *
     * \deprecated Use Backtraceable instead.
     */
    template <typename Tag_, typename StringType_ = std::string>
    class LIBEBT_DEPRECATED Backtracable : public Backtraceable<Tag_, StringType_>
    {
        protected:
            Backtracable() throw () LIBEBT_DEPRECATED ;

            Backtracable(const Backtracable & other) throw () LIBEBT_DEPRECATED ;

            virtual ~Backtracable() throw () LIBEBT_DEPRECATED ;
    };

    template <typename Tag_, typename StringType_>
    Backtracable<Tag_, StringType_>::Backtracable() throw () :
        Backtraceable<Tag_, StringType_>()
    {
    }

    template <typename Tag_, typename StringType_>
    Backtracable<Tag_, StringType_>::Backtracable(const Backtracable & other) throw () :
        Backtraceable<Tag_, StringType_>(other)
    {
    }

    template <typename Tag_, typename StringType_>
    Backtracable<Tag_, StringType_>::~Backtracable() throw ()
    {
    }
}

template <typename Tag_, typename StringType_>
StringType_
libebt::Backtraceable<Tag_, StringType_>::backtrace(const StringType_ & item_terminator) const
{
    StringType_ result;
    typename ListType::const_iterator p(_backtrace.begin()), end(_backtrace.end());
    for ( ; p != end ; ++p)
        result = result + *p + item_terminator;
    return result;
}

#endif
