/*
 * helpers.cpp              -- memory deallocating helpers
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
#include <cstdlib>
#include <cstring>

#include "helpers.hpp"

TMallocedString::TMallocedString(char* string)
    : String_(string)
{
}

TMallocedString::~TMallocedString()
{
    free(String_);
}

char* TMallocedString::c_str() const
{
    assert(String_);
    return String_;
}

TMallocedData::TMallocedData(void* data)
    : Data_(data)
{
}

TMallocedData::~TMallocedData()
{
    free(Data_);
}

void* TMallocedData::Data() const
{
    return Data_;
}

void* TMallocedData::Release()
{
    void* result = Data_;
    Data_ = NULL;
    return result;
}

TMallocedStringsArray::TMallocedStringsArray(char** strings)
    : Strings_(strings)
{
}

TMallocedStringsArray::~TMallocedStringsArray()
{
    free(Strings_);
}

char* TMallocedStringsArray::operator[](size_t pos) const
{
    assert(Strings_);
    return Strings_[pos];
}

size_t SizeOfCRecord(const NReinventedWheels::TBacktraceRecord& record)
{
    return sizeof(TBacktraceRecord)
        + record.Module_.size() + 1
        + record.Symbol_.size() + 1;
}

char* ConvertToCRecord(const NReinventedWheels::TBacktraceRecord& input,
    TBacktraceRecord* output, char* strings)
{
    output->Module_ = strings;
    strcpy(strings, input.Module_.c_str());
    strings += input.Module_.size() + 1;
    output->Symbol_ = strings;
    strcpy(strings, input.Symbol_.c_str());
    output->Offset_ = input.Offset_;
    output->Address_ = input.Address_;
    return strings + input.Symbol_.size() + 1;
}

TBacktraceRecord* ConvertToCRecord(
    const NReinventedWheels::TBacktraceRecord& input)
{
    TBacktraceRecord* output =
        reinterpret_cast<TBacktraceRecord*>(malloc(SizeOfCRecord(input)));
    if (output)
    {
        ConvertToCRecord(input, output, reinterpret_cast<char*>(output + 1));
    }
    return output;
}

