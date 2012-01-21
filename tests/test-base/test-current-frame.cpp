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

#include <stdexcept>

#include <backtrace/current-frame.hpp>
#include <backtrace/record.hpp>

#define BOOST_TEST_MODULE CurrentFrameTest
#include <boost/test/included/unit_test.hpp>

#include "exception-checker.hpp"

BOOST_AUTO_TEST_CASE(current_frame)
{
    BOOST_REQUIRE_EQUAL(NBacktrace::TDemangledBacktraceRecord(
        NBacktrace::GetCurrentFrame()).Function_,
        "current_frame::test_method()");
}

BOOST_AUTO_TEST_CASE(error_handling)
{
    BOOST_REQUIRE_EXCEPTION(NBacktrace::GetCurrentFrame(1000),
        std::logic_error,
        TExceptionChecker("offset is bigger than call stack"));
}

