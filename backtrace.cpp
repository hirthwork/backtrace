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
#include <dlfcn.h>

#include <cstddef>
#include <cstdlib>

#include <new>
#include <stdexcept>

#include "backtrace.h"
#include "record.h"

#include "backtrace.hpp"
#include "helpers.hpp"
#include "record.hpp"

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

int GetCurrentFrame(TBacktraceRecord* record, int offset)
{
    offset += 2;
    TMalloced frames(calloc(offset, sizeof(void*)));
    if(frames.Data())
    {
        int size = backtrace(static_cast<void**>(frames.Data()), offset);
        if(size == offset)
        {
            *record = ExtractBacktraceRecord(
                static_cast<void**>(frames.Data())[offset - 1]);
            return 1;
        }
    }
    return 0;
}

static void* GetFrames(int& size, int initialDepth)
{
    while (true)
    {
        TMalloced frames(calloc(initialDepth, sizeof(void*)));
        if(!frames.Data())
        {
            return NULL;
        }

        size = backtrace(static_cast<void**>(frames.Data()), initialDepth);
        if(size < initialDepth)
        {
            return frames.Release();
        }
        else
        {
            initialDepth <<= 1;
        }
    }
}

NReinventedWheels::TBacktrace NReinventedWheels::GetBacktrace(int offset,
    int initialDepth)
{
    int size;
    TMalloced frames(GetFrames(size, initialDepth + 2));
    if(!frames.Data())
    {
        throw std::bad_alloc();
    }

    if(size <= offset + 2)
    {
        throw std::logic_error("offset is bigger that call stack depth");
    }

    TBacktrace result(size - offset - 2);
    for (int i = offset + 2, j = 0; i < size; ++i, ++j)
    {
        result[j] =
            ExtractBacktraceRecord(static_cast<void**>(frames.Data())[i]);
    }
    return result;
}

TBacktraceRecord* GetBacktrace(int* size, int offset, int initialDepth)
{
    TMalloced frames(GetFrames(*size, initialDepth + 2));
    if(!frames.Data() || *size <= offset + 2)
    {
        return NULL;
    }

    TMalloced result(calloc(*size - offset - 2,
        sizeof(TBacktraceRecord)));
    if(!result.Data())
    {
        return NULL;
    }

    TBacktraceRecord* iter = static_cast<TBacktraceRecord*>(result.Data());
    for (int i = offset + 2; i < *size; ++i, ++iter)
    {
        *iter = ExtractBacktraceRecord(static_cast<void**>(frames.Data())[i]);
    }
    *size -= offset + 2;

    return reinterpret_cast<TBacktraceRecord*>(result.Release());
}

