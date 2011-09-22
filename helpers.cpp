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

#include "helpers.hpp"

namespace NReinventedWheels
{
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
        assert(Data_);
        return Data_;
    }

    TMallocedStringsArray::TMallocedStringsArray(char** strings)
        : Strings_(strings)
    {
    }

    TMallocedStringsArray::~TMallocedStringsArray()
    {
        free(Strings_);
    }

    char* TMallocedStringsArray::operator[](unsigned pos) const
    {
        assert(Strings_);
        return Strings_[pos];
    }
}

