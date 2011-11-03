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

#include <dlfcn.h>

#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include <new>
#include <stdexcept>

#include "backtrace.h"
#include "record.h"

#include "backtrace.hpp"
#include "frames.hpp"
#include "helpers.hpp"

static inline TBacktraceRecord ExtractBacktraceRecord(void* frame)
{
    Dl_info info;
    TBacktraceRecord record = {0, 0};
    if(frame && dladdr(frame, &info))
    {
        record.Module_ = info.dli_fname;
        record.Symbol_ = info.dli_sname;
    }
    else
    {
        record.Module_ = 0;
        record.Symbol_ = 0;
    }
    return record;
}

TBacktraceRecord NReinventedWheels::GetCurrentFrame(int offset)
{
    if (void* frame = GetFrame(offset + 1))
    {
        return ExtractBacktraceRecord(frame);
    }
    throw std::logic_error("offset is bigger that call stack depth");
}

int GetCurrentFrame(TBacktraceRecord* record, int offset)
{
    if (void* frame = GetFrame(offset + 1))
    {
        *record = ExtractBacktraceRecord(frame);
        return 1;
    }
    return 0;
}

NReinventedWheels::TBacktrace NReinventedWheels::GetBacktrace(int offset,
    int initialDepth)
{
    int size;
    void** data = GetFrames(offset + 1, initialDepth, &size);
    TMalloced frames(data);
    if(size <= 0)
    {
        throw std::logic_error("offset is bigger that call stack depth");
    }

    TBacktrace result(size);
    for (int i = 0; i < size; ++i)
    {
        result[i] = ExtractBacktraceRecord(*data++);
    }
    return result;
}

TBacktraceRecord* GetBacktrace(int* size, int offset, int initialDepth)
{
    void** data;
    try
    {
        data = GetFrames(offset + 1, initialDepth, size);
    }
    catch (const std::bad_alloc& exc)
    {
        errno = EDOM;
        return NULL;
    }
    TMalloced frames(data);
    if (*size <= 0)
    {
        errno = ERANGE;
        return NULL;
    }

    TBacktraceRecord* iter = static_cast<TBacktraceRecord*>(
        malloc(*size * sizeof(TBacktraceRecord)));
    if(!iter)
    {
        errno = EDOM;
        return NULL;
    }

    TMalloced result(iter);
    for (int i = 0; i < *size; ++i)
    {
        *iter++ = ExtractBacktraceRecord(*data++);
    }

    return reinterpret_cast<TBacktraceRecord*>(result.Release());
}

