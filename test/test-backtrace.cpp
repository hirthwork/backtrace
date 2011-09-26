/*
 * test-backtrace.cpp       -- backtrace functions tests
 *
 * Copyright (C) 2011 Dmitry Potapov <potapov.d@gmail.com>
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

#include <backtrace.h>
#include <record.h>

#include <backtrace.hpp>
#include <helpers.hpp>
#include <record.hpp>

#define BOOST_TEST_MODULE BacktraceTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_frame)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::GetCurrentFrame();
    BOOST_REQUIRE_EQUAL(record.Symbol_, "_ZN10test_frame11test_methodEv");
    BOOST_REQUIRE_EQUAL(record.Function_, "test_frame::test_method()");
    TBacktraceRecord* plainRecord = GetCurrentFrame(0);
    TMallocedData guard(plainRecord);
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_,
        "_ZN10test_frame11test_methodEv");
}

BOOST_AUTO_TEST_CASE(function_frame)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::GetCurrentFrame(-1);
    BOOST_REQUIRE_EQUAL(record.Function_,
        "NReinventedWheels::GetCurrentFrame(unsigned int, bool)");
    TBacktraceRecord* plainRecord = GetCurrentFrame(-1);
    TMallocedData guard(plainRecord);
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_, "GetCurrentFrame");
}

BOOST_AUTO_TEST_CASE(no_demangling)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::GetCurrentFrame(-1, false);
    BOOST_REQUIRE_EQUAL(record.Symbol_,
        "_ZN17NReinventedWheels15GetCurrentFrameEjb");
    BOOST_REQUIRE_EQUAL(record.Function_, "");

    NReinventedWheels::TBacktrace backtrace =
        NReinventedWheels::GetBacktrace(0, false);
    for (NReinventedWheels::TBacktrace::const_iterator iter =
        backtrace.begin(), end = backtrace.end(); iter != end; ++iter)
    {
        BOOST_REQUIRE_EQUAL(iter->Function_, "");
    }
}

int GetDepth()
{
    return GetStackDepth(1);
}

BOOST_AUTO_TEST_CASE(stack_depth)
{
    BOOST_REQUIRE_EQUAL(NReinventedWheels::GetStackDepth(), GetDepth() - 1);
    BOOST_REQUIRE_EQUAL(GetStackDepth(0), GetDepth() - 1);
}

BOOST_AUTO_TEST_CASE(backtrace_size)
{
    BOOST_REQUIRE_EQUAL(NReinventedWheels::GetStackDepth(),
        NReinventedWheels::GetBacktrace().size());
    BOOST_REQUIRE_EQUAL(GetStackDepth(0),
        NReinventedWheels::GetBacktrace().size());
}

void g(int)
{
    NReinventedWheels::TBacktrace backtrace =
        NReinventedWheels::GetBacktrace(-1);
    BOOST_REQUIRE_EQUAL(backtrace[0].Symbol_,
        "_ZN17NReinventedWheels12GetBacktraceEjb");
    BOOST_REQUIRE_EQUAL(backtrace[0].Function_,
        "NReinventedWheels::GetBacktrace(unsigned int, bool)");
    BOOST_REQUIRE_EQUAL(backtrace[1].Symbol_, "_Z1gi");
    BOOST_REQUIRE_EQUAL(backtrace[1].Function_, "g(int)");
    BOOST_REQUIRE_EQUAL(backtrace[2].Symbol_, "_Z1fv");
    BOOST_REQUIRE_EQUAL(backtrace[2].Function_, "f()");

    TBacktraceRecord** plainBacktrace = GetBacktrace(-2);
    TMallocedData guard(plainBacktrace);
    TBacktraceRecord** iter = plainBacktrace;
    BOOST_REQUIRE_EQUAL((*iter++)->Symbol_,
        "_ZN17NReinventedWheels12GetBacktraceEjb");
    BOOST_REQUIRE_EQUAL((*iter++)->Symbol_, "GetBacktrace");
    BOOST_REQUIRE_EQUAL((*iter++)->Symbol_, "_Z1gi");
    BOOST_REQUIRE_EQUAL((*iter++)->Symbol_, "_Z1fv");
    while (*iter)
    {
        ++iter;
    }

    BOOST_REQUIRE_EQUAL(iter - plainBacktrace, backtrace.size() + 1);
}

void f()
{
    g(5);
}

BOOST_AUTO_TEST_CASE(backtrace)
{
    f();
}

