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

#include <record.h>

#include <stdexcept>

#include <helpers.hpp>
#include <record.hpp>

#define BOOST_TEST_MODULE RecordTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(minimal)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::ParseBacktraceRecord("./prog() [0x8048871]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./prog");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "");
    BOOST_REQUIRE_EQUAL(record.Function_, "");
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0x8048871));

    record = NReinventedWheels::ParseBacktraceRecord("a() [0xb]");
    BOOST_REQUIRE_EQUAL(record.Module_, "a");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "");
    BOOST_REQUIRE_EQUAL(record.Function_, "");
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0xb));

    record = NReinventedWheels::ParseBacktraceRecord(
        "/usr/bin/test-backtrace() [0x8080e43]");
    BOOST_REQUIRE_EQUAL(record.Module_, "/usr/bin/test-backtrace");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "");
    BOOST_REQUIRE_EQUAL(record.Function_, "");
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0x8080e43));

    TBacktraceRecord* plainRecord = ParseBacktraceRecord("mod() [0x8048871]");
    TMallocedData guard(plainRecord);
    BOOST_REQUIRE_EQUAL(plainRecord->Module_, "mod");
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_, "");
    BOOST_REQUIRE_EQUAL(plainRecord->Address_,
        reinterpret_cast<void*>(0x8048871));

    plainRecord = ParseBacktraceRecord(
        "/tmp/hello[(+)]()+(+)()() [0x8048a8c]");
    TMallocedData secondGuard(plainRecord);
    BOOST_REQUIRE_EQUAL(plainRecord->Module_, "/tmp/hello[(+)]()+(+)()");
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_, "");
    BOOST_REQUIRE_EQUAL(plainRecord->Address_,
        reinterpret_cast<void*>(0x8048a8c));
}

BOOST_AUTO_TEST_CASE(plain_c)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::ParseBacktraceRecord(
            "/lib/libc.so.6(__libc_start_main+0xdc) [0xb7e38f9c]");
    BOOST_REQUIRE_EQUAL(record.Module_, "/lib/libc.so.6");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "__libc_start_main");
    BOOST_REQUIRE_EQUAL(record.Function_, "__libc_start_main");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0xdc);
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0xb7e38f9c));

    record = NReinventedWheels::ParseBacktraceRecord(
        "./prog(main+0x65) [0x80488fb]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./prog");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "main");
    BOOST_REQUIRE_EQUAL(record.Function_, "main");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0x65);
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0x80488fb));

    record = NReinventedWheels::ParseBacktraceRecord("a(_+0xc) [0xd]");
    BOOST_REQUIRE_EQUAL(record.Module_, "a");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "_");
    BOOST_REQUIRE_EQUAL(record.Function_, "_");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0xc);
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0xd));

    TBacktraceRecord* plainRecord =
        ParseBacktraceRecord("./prog(main+0x65) [0x80488fb]");
    TMallocedData guard(plainRecord);
    BOOST_REQUIRE_EQUAL(plainRecord->Module_, "./prog");
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_, "main");
    BOOST_REQUIRE_EQUAL(plainRecord->Offset_, 0x65);
    BOOST_REQUIRE_EQUAL(plainRecord->Address_,
        reinterpret_cast<void*>(0x80488fb));
}

BOOST_AUTO_TEST_CASE(demangling)
{
    NReinventedWheels::TBacktraceRecord record =
        NReinventedWheels::ParseBacktraceRecord(
            "./a.out(_Z1fv+0x2b) [0xface8d]");
    BOOST_REQUIRE_EQUAL(record.Module_, "./a.out");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "_Z1fv");
    BOOST_REQUIRE_EQUAL(record.Function_, "f()");
    BOOST_REQUIRE_EQUAL(record.Offset_, 43);
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0xface8d));

    record = NReinventedWheels::ParseBacktraceRecord(
        "/tmp/hello[(+)]()+(+)()(_ZN1ApLEi+0x1a) [0x8048bcc]");
    BOOST_REQUIRE_EQUAL(record.Module_, "/tmp/hello[(+)]()+(+)()");
    BOOST_REQUIRE_EQUAL(record.Symbol_, "_ZN1ApLEi");
    BOOST_REQUIRE_EQUAL(record.Function_, "A::operator+=(int)");
    BOOST_REQUIRE_EQUAL(record.Offset_, 0x1a);
    BOOST_REQUIRE_EQUAL(record.Address_, reinterpret_cast<void*>(0x8048bcc));
}

class TWhatChecker
{
    const std::string What;

public:
    TWhatChecker(const std::string& what)
        : What(what)
    {
    }

    bool operator()(const std::invalid_argument& exception) const
    {
        return What == exception.what();
    }
};

BOOST_AUTO_TEST_CASE(exceptions)
{
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("abcdefgh"),
        std::invalid_argument, TWhatChecker("line is too short: abcdefgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("abcde[fgh"),
        std::invalid_argument, TWhatChecker("no suitable opening square "
            "bracket found in backtrace string: abcde[fgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("abc[defgh"),
        std::invalid_argument, TWhatChecker("no suitable opening square "
            "bracket found in backtrace string: abc[defgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("abc(d[efgh"),
        std::invalid_argument, TWhatChecker("no suitable opening "
            "bracket found in backtrace string: abc(d[efgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("(abc[defgh"),
        std::invalid_argument, TWhatChecker("no suitable opening "
            "bracket found in backtrace string: (abc[defgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("a(bcd[efgh"),
        std::invalid_argument, TWhatChecker("no plus sign found in backtrace "
            "string: a(bcd[efgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("a(+cd[efgh"),
        std::invalid_argument, TWhatChecker("no plus sign found in backtrace "
            "string: a(+cd[efgh"));
    BOOST_REQUIRE_EXCEPTION(
        NReinventedWheels::ParseBacktraceRecord("a(c+d[efgh"),
        std::invalid_argument, TWhatChecker("no plus sign found in backtrace "
            "string: a(c+d[efgh"));

    BOOST_REQUIRE(ParseBacktraceRecord("abcdefgh") == NULL);
}

