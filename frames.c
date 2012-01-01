/*
 * frames.c                 -- low-level frames extracting functions
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

#include "config.h"
#include "frames.h"
#include "unwind.h"

struct TTrace {
    void** Backtrace;
    int Size;
    int Allocated;
    void* LastEbp;
    void* LastEsp;
};

struct TFrame {
    void* Frame;
    int Position;
    void* LastEbp;
    void* LastEsp;
};

struct TLayout {
    struct TLayout* Next;
    void* ReturnAddress;
};

static BACKTRACE_INLINE void Append(struct TTrace* trace, void* ip)
{
    if (trace->Size == trace->Allocated)
    {
        void* backtrace = trace->Backtrace;
        trace->Allocated <<= 1;
        trace->Backtrace =
            realloc(trace->Backtrace, sizeof(void*) * trace->Allocated);
        if (!trace->Backtrace)
        {
            free(backtrace);
            errno = EDOM;
            return;
        }
    }
    if (trace->Size >= 0)
    {
        trace->Backtrace[trace->Size] = ip;
    }
    ++trace->Size;
}

static _Unwind_Reason_Code BacktraceHandler(struct _Unwind_Context* context,
    void* p)
{
    struct TTrace* trace = p;
    if (trace->Size >= 0)
    {
        Append(trace, (void*)(_Unwind_GetIP(context)));
    }
    else
    {
        ++trace->Size;
    }
    trace->LastEbp = (void*)(_Unwind_GetGR(context, 5));
    trace->LastEsp = (void*)(_Unwind_GetCFA(context));
    return trace->Backtrace ? _URC_NO_REASON : _URC_END_OF_STACK;
}

void** GetFrames(unsigned offset, unsigned initialDepth, int* size)
{
    struct TTrace trace;
    trace.Backtrace = malloc(sizeof(void*) * initialDepth);
    trace.Size = -offset - 1;
    trace.Allocated = initialDepth;
    if(!trace.Backtrace)
    {
        errno = EDOM;
    }
    else
    {
        struct TLayout* ebp;
        _Unwind_Backtrace(BacktraceHandler, &trace);
        ebp = trace.LastEbp;
        while(trace.Backtrace && (void*)ebp >= trace.LastEsp
            && !((long)(ebp) & 3))
        {
            Append(&trace, ebp->ReturnAddress);
            ebp = ebp->Next;
        }
        if (trace.Size > 0)
        {
            *size = trace.Size;
        }
        else
        {
            errno = ERANGE;
            free(trace.Backtrace);
            trace.Backtrace = 0;
        }
    }
    return trace.Backtrace;
}

static _Unwind_Reason_Code FrameHandler(struct _Unwind_Context* context,
    void* p)
{
    struct TFrame* frame = p;
    ++frame->Position;
    if (frame->Position > 0)
    {
        frame->Frame = (void*)(_Unwind_GetIP(context));
        return _URC_END_OF_STACK;
    }
    else
    {
        frame->LastEbp = (void*)(_Unwind_GetGR(context, 5));
        frame->LastEsp = (void*)(_Unwind_GetCFA(context));
        return _URC_NO_REASON;
    }
}

void* GetFrame(unsigned offset)
{
    struct TFrame frame;
    frame.Position = -offset - 1;
    _Unwind_Backtrace(FrameHandler, &frame);
    if (frame.Position > 0)
    {
        return frame.Frame;
    }
    else
    {
        struct TLayout* ebp = frame.LastEbp;
        while((void*)ebp >= frame.LastEsp && !((long)ebp & 3))
        {
            if (frame.Position >= 0)
            {
                return ebp->ReturnAddress;
            }
            ++frame.Position;
            ebp = ebp->Next;
        }

        return 0;
    }
}

