/*
 * backtrace-c.c            -- backtrace helper
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

#include <errno.h>
#include <stdlib.h>

#include "backtrace.h"
#include "extract.h"
#include "frames.h"
#include "record.h"

int GetCurrentFrame(struct TBacktraceRecord* record, int offset)
{
    void* frame = GetFrame(offset + 1);
    if (frame)
    {
        *record = ExtractBacktraceRecord(frame);
        return 1;
    }
    return 0;
}

struct TBacktraceRecord* GetBacktrace(int* size, int offset, int initialDepth)
{
    void** data = GetFrames(offset + 1, initialDepth, size);
    struct TBacktraceRecord* result = 0;
    if (data)
    {
        struct TBacktraceRecord* iter =
            malloc(*size * sizeof(struct TBacktraceRecord));
        result = iter;
        if (!result)
        {
            errno = EDOM;
        }
        else
        {
            void** dataIter = data;
            int i = 0;
            for (; i < *size; ++i)
            {
                *iter++ = ExtractBacktraceRecord(*dataIter++);
            }
        }
        free(data);
    }
    return result;
}

