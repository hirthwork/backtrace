/*
 * helpers.hpp              -- memory deallocating helpers
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

#ifndef __HELPERS_HPP_2011_09_20__
#define __HELPERS_HPP_2011_09_20__

namespace NReinventedWheels
{
    class TMallocedString
    {
        char* const String_;

    public:
        TMallocedString(char* string);
        ~TMallocedString();
        char* c_str() const;
    };

    class TMallocedData
    {
        void* const Data_;

    public:
        TMallocedData(void* data);
        ~TMallocedData();
        void* Data() const;
    };

    class TMallocedStringsArray
    {
        char** const Strings_;

    public:
        TMallocedStringsArray(char** strings);
        ~TMallocedStringsArray();
        char* operator[](unsigned pos) const;
    };
}

#endif

