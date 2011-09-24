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

#include <cassert>
#include <cstring>
#include <ios>
#include <sstream>
#include <string>

#include <cxxabi.h>

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
// ./prog [0x8048871]
// ./prog(myfunc+0x21) [0x8048894]
// /usr/bin/test-backtrace() [0x8080e43]
NReinventedWheels::TBacktraceRecord NReinventedWheels::ParseBacktraceRecord(
    const std::string& record)
{
    TBacktraceRecord result;
    size_t openingBracket = record.find('(');
    size_t space;
    if (openingBracket == record.npos)
    {
        space = record.find(' ');
        assert(space != record.npos);
        result.Module_ = std::string(record.c_str(), space);
    }
    else if (record[openingBracket + 1] == ')')
    {
        space = openingBracket + 2;
        result.Module_ = std::string(record.c_str(), openingBracket);
    }
    else
    {
        result.Module_ = std::string(record.c_str(), openingBracket);
        // the minimal statement in brackets should looks like
        // (a+0xb)
        // 0123456 <- offsets from opening bracket
        // maximum offset representation length should be not greater that 18
        size_t closingBracket = record.find(')');
        assert(closingBracket != record.npos);
        assert(closingBracket - openingBracket >= 6);
        size_t plus = record.find('+');
        assert(plus != record.npos);
        assert(plus - openingBracket >= 2);
        assert(closingBracket - plus >= 4);
        assert(closingBracket - plus <= 18 + 1);
        // step over the bracket
        ++openingBracket;
        int status;
        result.Symbol_ = std::string(record.c_str() + openingBracket,
            plus - openingBracket);
        TMallocedString demangledName(abi::__cxa_demangle(
            result.Symbol_.c_str(), NULL, NULL, &status));
        result.Function_ = status ? result.Symbol_ : demangledName.c_str();
        // step over the plus sign
        ++plus;
        assert(record[plus] == '0');
        assert(record[plus + 1] == 'x');
        result.Offset_ =
            ParseHex(record.c_str() + plus, closingBracket - plus);
        space = closingBracket + 1;
    }
    assert(record[space] == ' ');

    // step over the space
    ++space;
    assert(record[space] == '[');
    // step over the bracket
    ++space;
    size_t closingBracket = record.find(']');
    assert(closingBracket != record.npos);
    result.Address_ = ParseHex<void*>(record.c_str() + space, closingBracket - space);
    return result;
}

