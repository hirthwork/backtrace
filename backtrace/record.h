/*
 * record.h                 -- backtrace information atom
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

#ifndef __RECORD_H_2011_09_24__
#define __RECORD_H_2011_09_24__

struct TBacktraceRecord
{
    /* Frame pointer, always valid and can be passed to addr2line */
    const void* Frame_;
    /* NULL indicates error during record extraction */
    const char* Module_;
    /* Address where module is loaded */
    const void* ModuleAddress_;
    /* NULL indicates symbol information absence */
    const char* Symbol_;
    /* Symbol address, can be used for offset calculation */
    const void* SymbolAddress_;
};

#endif

