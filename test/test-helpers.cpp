/*
 * test-helpers.cpp         -- helper functions tests
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

#include <helpers.hpp>
#include <record.hpp>

#define BOOST_TEST_MODULE HelpersTest
#include <boost/test/unit_test.hpp>

static NReinventedWheels::TBacktraceRecord CreateRecord()
{
    NReinventedWheels::TBacktraceRecord record;
    record.Module_ = "./prog";
    record.Symbol_ = "func";
    record.Offset_ = 0x1a;
    record.Address_ = reinterpret_cast<void*>(0xface8d);
    return record;
}

BOOST_AUTO_TEST_CASE(size)
{
    BOOST_REQUIRE_EQUAL(SizeOfCRecord(CreateRecord()),
        sizeof(TBacktraceRecord) + sizeof("./prog\0func"));
}

BOOST_AUTO_TEST_CASE(convert_in_place)
{
    char buffer[64];
    NReinventedWheels::TBacktraceRecord record = CreateRecord();
    BOOST_REQUIRE_LT(SizeOfCRecord(record), sizeof(buffer));
    TBacktraceRecord plainRecord;
    BOOST_REQUIRE_EQUAL(SizeOfCRecord(record) - sizeof(TBacktraceRecord),
        ConvertToCRecord(record, &plainRecord, buffer) - buffer);
    BOOST_REQUIRE_EQUAL(plainRecord.Module_, record.Module_);
    BOOST_REQUIRE_EQUAL(buffer, record.Module_);
    BOOST_REQUIRE_EQUAL(plainRecord.Symbol_, record.Symbol_);
    BOOST_REQUIRE_EQUAL(buffer + record.Module_.size() + 1, record.Symbol_);
    BOOST_REQUIRE_EQUAL(plainRecord.Offset_, record.Offset_);
    BOOST_REQUIRE_EQUAL(plainRecord.Address_, record.Address_);
}

BOOST_AUTO_TEST_CASE(convert)
{
    NReinventedWheels::TBacktraceRecord record = CreateRecord();
    TBacktraceRecord* plainRecord = ConvertToCRecord(record);
    TMallocedData guard(plainRecord);
    char* buffer = reinterpret_cast<char*>(plainRecord + 1);
    BOOST_REQUIRE(plainRecord != NULL);
    BOOST_REQUIRE_EQUAL(plainRecord->Module_, record.Module_);
    BOOST_REQUIRE_EQUAL(buffer, record.Module_);
    BOOST_REQUIRE_EQUAL(plainRecord->Symbol_, record.Symbol_);
    BOOST_REQUIRE_EQUAL(buffer + record.Module_.size() + 1, record.Symbol_);
    BOOST_REQUIRE_EQUAL(plainRecord->Offset_, record.Offset_);
    BOOST_REQUIRE_EQUAL(plainRecord->Address_, record.Address_);
}

