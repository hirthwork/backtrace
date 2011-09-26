/*
 * record.cpp               -- backtrace_symbols parsing routines
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

#include <cstring>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>

#include <cxxabi.h>

#include "record.h"

#include "helpers.hpp"
#include "record.hpp"

template <class T>
static T ParseHex(const char* string, size_t length)
{
    T t;
    std::istringstream stream(std::string(string, length));
    std::hex(stream);
    stream >> t;
    return t;
}

template <class T>
static T ParseHex(const char* string)
{
    return ParseHex<T>(string, strlen(string));
}

class THexParser
{
    const char* const String_;
    const size_t Length_;

public:
    THexParser(const char* string, size_t length)
        : String_(string)
        , Length_(length)
    {
    }

    template <class T>
    operator T() const
    {
        return ParseHex<T>(String_, Length_);
    }
};

// return type overloading
static THexParser ParseHex(const char* string, size_t length)
{
    return THexParser(string, length);
}

/*static THexParser ParseHex(const char* string)
{
    return THexParser(string, strlen(string));
}*/

// input string should have one of the two forms:
// ./prog() [0x8048871]
// ./prog(myfunc+0x21) [0x8048894]
// /usr/bin/test-backtrace() [0x8080e43]
// /tmp/hello[(+)]()+(+)()(_ZN1ApLEi+0x1a) [0x8048bcc]
// /tmp/hello[(+)]()+(+)()() [0x8048a8c]
//
NReinventedWheels::TBacktraceRecord NReinventedWheels::ParseBacktraceRecord(
    const std::string& record, bool demangle)
{
    if (record.size() < 9)
    {
        throw std::invalid_argument("line is too short: " + record);
    }

    TBacktraceRecord result;
    size_t squareBracket = record.rfind('[', record.size() - 5);
    if (squareBracket == record.npos || squareBracket < 4)
    {
        throw std::invalid_argument(
            "no suitable opening square bracket found in backtrace string: "
            + record);
    }

    result.Address_ = ParseHex<void*>(record.c_str() + squareBracket + 3,
        record.size() - squareBracket - 4);
    size_t bracket = record.rfind('(', squareBracket - 3);
    if (bracket == record.npos || !bracket)
    {
        throw std::invalid_argument(
            "no suitable opening bracket found in backtrace string: "
            + record);
    }

    if (bracket < squareBracket - 3)
    {
        size_t plus = record.find('+', bracket + 2);
        if (plus == record.npos || plus + 6 > squareBracket)
        {
            throw std::invalid_argument(
                "no plus sign found in backtrace string: " + record);
        }
        result.Symbol_ = std::string(record, bracket + 1, plus - bracket - 1);
        if (demangle) {
            int status;
            TMallocedString demangledName(abi::__cxa_demangle(
                result.Symbol_.c_str(), NULL, NULL, &status));
            result.Function_ =
                status ? result.Symbol_ : demangledName.c_str();
        }
        result.Offset_ = ParseHex(record.c_str() + plus + 3,
            squareBracket - plus - 5);
    }
    result.Module_ = std::string(record.c_str(), bracket);
    return result;
}

TBacktraceRecord* ParseBacktraceRecord(const char* record)
{
    try {
        return ConvertToCRecord(
            NReinventedWheels::ParseBacktraceRecord(record));
    }
    catch (...)
    {
        return NULL;
    }
}

