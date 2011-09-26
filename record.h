/*
 * record.h                 -- backtrace_symbols parsing C interface
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

#ifndef __RECORD_H_2011_09_24__
#define __RECORD_H_2011_09_24__

struct TBacktraceRecord
{
    const char* Module_;
    // empty string indicates symbol information absence
    const char* Symbol_;
    // TODO: is this really required?
    unsigned Offset_;
    const void* Address_;
};

#ifdef  __cplusplus
extern "C"
#endif
// returns pointer to malloced object, NULL on failure
struct TBacktraceRecord* ParseBacktraceRecord(const char* record);

#endif

