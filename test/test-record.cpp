/*
 * test-record.cpp          -- backtrace_symbols parser tests
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

#include <record.hpp>

#define BOOST_TEST_MODULE RecordTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(record1)
{
    TBacktraceRecord record = {"file", "_ZN10test_frame11test_methodEv"};
    NReinventedWheels::TDemangledBacktraceRecord demangled(record);
    BOOST_REQUIRE_EQUAL(demangled.Module_, "file");
    BOOST_REQUIRE_EQUAL(demangled.Symbol_, "_ZN10test_frame11test_methodEv");
    BOOST_REQUIRE_EQUAL(demangled.Function_, "test_frame::test_method()");
}

BOOST_AUTO_TEST_CASE(record2)
{
    TBacktraceRecord record = {NULL,
        "_ZN17NReinventedWheels15GetCurrentFrameEjb"};
    NReinventedWheels::TDemangledBacktraceRecord demangled(record);
    BOOST_REQUIRE(demangled.Module_ == NULL);
    BOOST_REQUIRE_EQUAL(demangled.Symbol_,
        "_ZN17NReinventedWheels15GetCurrentFrameEjb");
    BOOST_REQUIRE_EQUAL(demangled.Function_,
        "NReinventedWheels::GetCurrentFrame(unsigned int, bool)");
}

BOOST_AUTO_TEST_CASE(record3)
{
    TBacktraceRecord record = {"", "GetCurrentFrame"};
    NReinventedWheels::TDemangledBacktraceRecord demangled(record);
    BOOST_REQUIRE_EQUAL(demangled.Module_, "");
    BOOST_REQUIRE_EQUAL(demangled.Symbol_, "GetCurrentFrame");
    BOOST_REQUIRE_EQUAL(demangled.Function_, "GetCurrentFrame");
}

BOOST_AUTO_TEST_CASE(record4)
{
    TBacktraceRecord record = {NULL, NULL};
    NReinventedWheels::TDemangledBacktraceRecord demangled(record);
    BOOST_REQUIRE(demangled.Module_ == NULL);
    BOOST_REQUIRE(demangled.Symbol_ == NULL);
    BOOST_REQUIRE_EQUAL(demangled.Function_, "");
}

