/*
 * extract.h                -- TBacktraceRecord extraction routine
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

#ifndef __EXTRACT_H_31_12_2011__
#define __EXTRACT_H_31_12_2011__

#include <dlfcn.h>

#include "config.h"
#include "record.h"

static BACKTRACE_INLINE struct TBacktraceRecord ExtractBacktraceRecord(
    void* frame)
{
    Dl_info info;
    struct TBacktraceRecord record = {0, 0, 0, 0, 0};
    if(frame && dladdr(frame, &info))
    {
        record.Frame_ = frame;
        record.Module_ = info.dli_fname;
        record.ModuleAddress_ = info.dli_fbase;
        record.Symbol_ = info.dli_sname;
        record.SymbolAddress_ = info.dli_saddr;
    }
    return record;
}

#endif

