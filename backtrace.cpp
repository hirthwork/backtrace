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

#include <cstdlib>

#include "backtrace.h"
#include "record.h"

#include "backtrace.hpp"
#include "helpers.hpp"
#include "record.hpp"

NReinventedWheels::TBacktraceRecord NReinventedWheels::GetCurrentFrame(
    unsigned offset, bool demangle)
{
    int count = offset + 2;
    TMallocedData frames(calloc(count, sizeof(void*)));
    int size = backtrace(static_cast<void**>(frames.Data()), count);
    TMallocedStringsArray strings(
        backtrace_symbols(static_cast<void**>(frames.Data()), size));
    return ParseBacktraceRecord(strings[size - 1], demangle);
}

int NReinventedWheels::GetStackDepth(int initialDepth)
{
    while (true)
    {
        TMallocedData frames(calloc(initialDepth, sizeof(void*)));
        int size =
            backtrace(static_cast<void**>(frames.Data()), initialDepth);
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

NReinventedWheels::TBacktrace NReinventedWheels::GetBacktrace(unsigned offset,
    bool demangle)
{
    int depth = GetStackDepth();
    TMallocedData frames(calloc(depth, sizeof(void*)));
    int size = backtrace(static_cast<void**>(frames.Data()), depth);
    TMallocedStringsArray strings(
        backtrace_symbols(static_cast<void**>(frames.Data()), size));
    TBacktrace backtrace;
    for (int i = offset + 1; i < size; ++i)
    {
        backtrace.push_back(ParseBacktraceRecord(strings[i], demangle));
    }
    return backtrace;
}

TBacktraceRecord* GetCurrentFrame(unsigned offset)
{
    return ConvertToCRecord(
        NReinventedWheels::GetCurrentFrame(offset + 1, false));
}

int GetStackDepth(int initialDepth)
{
    return NReinventedWheels::GetStackDepth(initialDepth + 1) - 1;
}

TBacktraceRecord** GetBacktrace(unsigned offset)
{
    NReinventedWheels::TBacktrace backtrace =
        NReinventedWheels::GetBacktrace(offset + 1, false);
    size_t size = (backtrace.size() + 1) * sizeof(TBacktraceRecord*);
    for (NReinventedWheels::TBacktrace::const_iterator iter =
        backtrace.begin(), end = backtrace.end(); iter != end; ++iter)
    {
        size += SizeOfCRecord(*iter);
    }

    TMallocedData result(malloc(size));
    if (result.Data())
    {
        TBacktraceRecord** pointers =
            reinterpret_cast<TBacktraceRecord**>(result.Data());
        pointers[backtrace.size()] = NULL;
        TBacktraceRecord* data = reinterpret_cast<TBacktraceRecord*>(
            pointers + backtrace.size() + 1);
        char* strings = reinterpret_cast<char*>(data + backtrace.size());
        for (NReinventedWheels::TBacktrace::const_iterator iter =
            backtrace.begin(), end = backtrace.end(); iter != end; ++iter)
        {
            *pointers++ = data;
            strings = ConvertToCRecord(*iter, data++, strings);
        }
    }
    return reinterpret_cast<TBacktraceRecord**>(result.Release());
}

