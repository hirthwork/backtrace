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
using NReinventedWheels::TBacktraceRecord;
using NReinventedWheels::ParseBacktraceRecord;

#define BOOST_TEST_MODULE RecordTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(minimal)
{
    TBacktraceRecord record = ParseBacktraceRecord("./prog [0x8048871]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./prog");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "");
    BOOST_REQUIRE_EQUAL(record.Function_, "");
    BOOST_REQUIRE_EQUAL(record.Address_, (void*)0x8048871);

    record = ParseBacktraceRecord("/usr/bin/test-backtrace() [0x8080e43]");
    BOOST_REQUIRE_EQUAL(record.Module_, "/usr/bin/test-backtrace");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "");
    BOOST_REQUIRE_EQUAL(record.Function_, "");
    BOOST_REQUIRE_EQUAL(record.Address_, (void*)0x8080e43);
}

BOOST_AUTO_TEST_CASE(plain_c)
{
    TBacktraceRecord record = ParseBacktraceRecord(
        "/lib/libc.so.6(__libc_start_main+0xdc) [0xb7e38f9c]");
    BOOST_REQUIRE_EQUAL(record.Module_, "/lib/libc.so.6");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "__libc_start_main");
    BOOST_REQUIRE_EQUAL(record.Function_, "__libc_start_main");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0xdc);
    BOOST_REQUIRE_EQUAL(record.Address_, (void*)0xb7e38f9c);

    record = ParseBacktraceRecord("./prog(main+0x65) [0x80488fb]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./prog");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "main");
    BOOST_REQUIRE_EQUAL(record.Function_, "main");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0x65);
    BOOST_REQUIRE_EQUAL(record.Address_, (void*)0x80488fb);
}

BOOST_AUTO_TEST_CASE(demangling)
{
    TBacktraceRecord record =
        ParseBacktraceRecord("./a.out(_Z1fv+0x2b) [0xface8d]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./a.out");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "_Z1fv");
    BOOST_REQUIRE_EQUAL(record.Function_, "f()");
    BOOST_REQUIRE_EQUAL(record.Offset_, 43);
    BOOST_REQUIRE_EQUAL(record.Address_, (void*)0xface8d);
}

