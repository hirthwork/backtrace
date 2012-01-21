/*
 * backtrace.cpp            -- backtrace helper
 *
 * Copyright (C) 2011, 2012 Dmitry Potapov <potapov.d@gmail.com>
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

#include <cerrno>

#include <new>
#include <stdexcept>
// this include required for std::logic_error construction, as it accepts
// const reference to std::string
#include <string>

#include "extract.h"
#include "frames.h"
#include "record.h"

#include "backtrace.hpp"
#include "helpers.hpp"

NBacktrace::TBacktrace NBacktrace::GetBacktrace(int offset,
    int initialDepth)
{
    int size;
    void** data = GetFrames(offset + 1, initialDepth, &size);
    if (!data)
    {
        if (errno == EDOM)
        {
            throw std::bad_alloc();
        }
        else
        {
            throw std::logic_error("offset is bigger than call stack");
        }
    }

    TMalloced guard(data);
    TBacktrace result(size);
    for (int i = 0; i < size; ++i)
    {
        result[i] = ExtractBacktraceRecord(*data++);
    }
    return result;
}

