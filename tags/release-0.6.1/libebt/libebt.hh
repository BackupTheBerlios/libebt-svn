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
#ifndef HAVE_THE_LIBEBT_LIBEBT_HH
#define HAVE_THE_LIBEBT_LIBEBT_HH 1

#include <libebt/libebt_util.hh>
#include <libebt/libebt_version.hh>

#include <list>
#include <string>
#include <algorithm>
#include <iterator>

/** \file
 * Definitions for the BacktraceContext and Backtracable classes.
 */

namespace libebt
{
    /**
     * Represents an item on the backtrace context stack. When a
     * BacktraceContext is instantiated, a context stack entry is acquired. When
     * the BacktraceContext is destroyed, the stack entry is removed.
     *
     * The Tag_ template parameter should be an empty struct. It is used for
     * backtrace grouping.
     *
     * The StringType_ template parameter specifies the type of string used for
     * messages.
     */
    template <typename Tag_, typename StringType_ = std::string>
    class BacktraceContext
    {
        private:
            /**
             * Our backtrace list type. Internal use only.
             */
            typedef std::list<StringType_> ListType;

            /**
             * Return the backtrace stack. Internal use only.
             */
            inline static ListType * _get_list()
            {
                static ListType list;
                return &list;
            }

            /**
             * Do not copy.
             */
            BacktraceContext(const BacktraceContext & other)
            {
            }

            /**
             * Our item in the backtrace stack.
             */
            typename ListType::iterator _our_item;

        public:
            /**
             * Constructor.
             *
             * \param context The context message.
             */
            inline BacktraceContext(const StringType_ & context) :
                _our_item(_get_list()->insert(_get_list()->end(), context))
            {
            }

            /**
             * Destructor.
             */
            inline ~BacktraceContext()
            {
                _get_list()->pop_back();
            }

            /**
             * Change context. Note that this will work even if our context
             * item isn't top of the context stack (for example, if there are
             * two context objects in scope).
             *
             * \param context The new context message.
             */
            inline void change_context(const StringType_ & context)
            {
                *_our_item = context;
            }

            /**
             * Create and return a backtrace string by joining together context
             * information.
             *
             * \param item_terminator A terminator which is appended to every
             *                        context item. Defaults to a newline.
             */
            static StringType_ backtrace(const StringType_ & item_terminator =
                    newline_string<StringType_>());

            /**
             * Copy backtrace items to the insert iterator I_.
             */
            template <typename I_>
            static void copy_backtrace_items_to(I_ i)
            {
                std::copy(_get_list()->begin(), _get_list()->end(), i);
            }
    };

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
    class Backtracable
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
            Backtracable() throw () :
                _backtrace()
            {
                BacktraceContext<Tag_, StringType_>::copy_backtrace_items_to(
                        std::back_inserter(_backtrace));
            }

            /**
             * Copy constructor.
             */
            Backtracable(const Backtracable & other) throw () :
                _backtrace(other._backtrace.begin(), other._backtrace.end())
            {
            }

            /**
             * Destructor.
             */
            virtual ~Backtracable() throw ()
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
}

template <typename Tag_, typename StringType_>
StringType_
libebt::BacktraceContext<Tag_, StringType_>::backtrace(const StringType_ & item_terminator)
{
    StringType_ result;
    typename ListType::const_iterator p(_get_list()->begin()), end(_get_list()->end());
    for ( ; p != end ; ++p)
        result = result + *p + item_terminator;
    return result;
}

template <typename Tag_, typename StringType_>
StringType_
libebt::Backtracable<Tag_, StringType_>::backtrace(const StringType_ & item_terminator) const
{
    StringType_ result;
    typename ListType::const_iterator p(_backtrace.begin()), end(_backtrace.end());
    for ( ; p != end ; ++p)
        result = result + *p + item_terminator;
    return result;
}

#endif
