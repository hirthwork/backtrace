/*
 * extract.c                -- backtrace information extracting routine
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

#include "extract.h"

TBacktraceRecord ExtractBacktraceRecord(void* frame)
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

