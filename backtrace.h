/*
 * backtrace.h              -- backtrace helper C interface
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

#ifndef __BACKTRACE_H_2011_09_24__
#define __BACKTRACE_H_2011_09_24__

#include "record.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* returns zero on failure, non-zero on success */
extern int GetCurrentFrame(TBacktraceRecord* record, int offset);

/* returns NULL on failure */
extern TBacktraceRecord* GetBacktrace(int* size, int offset,
    int initialDepth);

#ifdef  __cplusplus
}
#endif

#endif

