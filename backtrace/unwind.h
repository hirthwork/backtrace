/*
 * unwind.h                 -- required unwind functions prototypes
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

#ifndef __UNWIND_H_2011_11_26__
#define __UNWIND_H_2011_11_26__

#ifdef __cplusplus
extern "C" {
#endif

struct _Unwind_Context;
extern unsigned _Unwind_GetIP(struct _Unwind_Context*);
extern unsigned _Unwind_GetGR(struct _Unwind_Context*, int);
extern unsigned _Unwind_GetCFA(struct _Unwind_Context*);

typedef enum
{
  _URC_NO_REASON = 0,
  _URC_END_OF_STACK = 5
} _Unwind_Reason_Code;

extern _Unwind_Reason_Code _Unwind_Backtrace(
    _Unwind_Reason_Code (*)(struct _Unwind_Context*, void*), void*);

#ifdef __cplusplus
}
#endif

#endif


