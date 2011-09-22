/*
 * backtrace.cpp            -- backtrace helper
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

#include <execinfo.h>

#include <cassert>
#include <cstdlib>

#include "backtrace.hpp"
#include "helpers.hpp"
#include "record.hpp"

NReinventedWheels::TBacktraceRecord NReinventedWheels::GetCurrentFrame(
    unsigned offset)
{
    int count = offset + 2;
    TMallocedData frames(calloc(count, sizeof(void*)));
    int size = backtrace(static_cast<void**>(frames.Data()), count);
    assert(size == count);
    TMallocedStringsArray strings(
        backtrace_symbols(static_cast<void**>(frames.Data()), size));
    return ParseBacktraceRecord(strings[size - 1]);
}

int NReinventedWheels::GetStackDepth(int initialDepth)
{
    while (true)
    {
        TMallocedData frames(calloc(initialDepth, sizeof(void*)));
        int size =
            backtrace(static_cast<void**>(frames.Data()), initialDepth);
        assert(size > 0);
        if (size < initialDepth)
        {
            return size - 1;
        }
        else
        {
            initialDepth <<= 1;
        }
    }
}

NReinventedWheels::TBacktrace NReinventedWheels::GetBacktrace(unsigned offset)
{
    int depth = GetStackDepth();
    TMallocedData frames(calloc(depth, sizeof(void*)));
    int size = backtrace(static_cast<void**>(frames.Data()), depth);
    assert(size == depth);
    TMallocedStringsArray strings(
        backtrace_symbols(static_cast<void**>(frames.Data()), size));
    TBacktrace backtrace;
    for (int i = offset + 1; i < size; ++i)
    {
        backtrace.push_back(ParseBacktraceRecord(strings[i]));
    }
    return backtrace;
}

