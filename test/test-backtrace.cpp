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

#include <backtrace.hpp>
#include <record.hpp>
using NReinventedWheels::TBacktraceRecord;
using NReinventedWheels::GetCurrentFrame;
using NReinventedWheels::GetStackDepth;
using NReinventedWheels::GetBacktrace;

#define BOOST_TEST_MODULE RecordTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_frame)
{
    TBacktraceRecord record = GetCurrentFrame();
    BOOST_REQUIRE_EQUAL(record.Function_, "test_frame::test_method()");
}

BOOST_AUTO_TEST_CASE(function_frame)
{
    TBacktraceRecord record = GetCurrentFrame(-1);
    BOOST_REQUIRE_EQUAL(record.Function_,
        "NReinventedWheels::GetCurrentFrame(unsigned int)");
}

int GetDepth()
{
    return GetStackDepth(1);
}

BOOST_AUTO_TEST_CASE(stack_depth)
{
    BOOST_REQUIRE_EQUAL(GetStackDepth(), GetDepth() - 1);
}

BOOST_AUTO_TEST_CASE(backtrace_size)
{
    BOOST_REQUIRE_EQUAL(GetStackDepth(), GetBacktrace().size());
}

