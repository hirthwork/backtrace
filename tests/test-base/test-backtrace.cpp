/*
 * test-backtrace.cpp       -- backtrace functions tests
 *
 * Copyright (C) 2011, 2012 Dmitry Potapov <potapov.d@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <new>
#include <stdexcept>

#include <backtrace/backtrace.hpp>

#define BOOST_TEST_MODULE BacktraceTest
#include <boost/test/included/unit_test.hpp>

#include "exception-checker.hpp"

void g(int)
{
    NBacktrace::TBacktrace backtrace =
        NBacktrace::GetBacktrace(-1, 1);
    BOOST_REQUIRE_EQUAL(backtrace[0].Symbol_,
        "_ZN10NBacktrace12GetBacktraceEii");
    BOOST_REQUIRE_EQUAL(backtrace[1].Symbol_, "_Z1gi");
#ifdef __FreeBSD__
    BOOST_REQUIRE_EQUAL(backtrace[2].Symbol_, "_Z1gi");
#else
    BOOST_REQUIRE(backtrace[2].Symbol_ == NULL);
#endif
    BOOST_REQUIRE_EQUAL(backtrace[3].Symbol_,
        "_ZN9backtrace11test_methodEv");
}

static void f()
{
    g(5);
}

BOOST_AUTO_TEST_CASE(backtrace)
{
    f();
    BOOST_REQUIRE_EQUAL(NBacktrace::GetBacktrace()[0].Symbol_,
        "_ZN9backtrace11test_methodEv");
}

BOOST_AUTO_TEST_CASE(error_handling)
{
    BOOST_REQUIRE_EXCEPTION(NBacktrace::GetBacktrace(100),
        std::logic_error,
        TExceptionChecker("offset is bigger than call stack"));
    BOOST_REQUIRE_EXCEPTION(NBacktrace::GetBacktrace(0, 0x7fffffff),
        std::bad_alloc,
        TExceptionChecker("std::bad_alloc"));
}

