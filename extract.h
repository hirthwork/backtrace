/*
 * extract.h                -- backtrace information extracting routine
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

#ifndef __EXTRACT_H_2011_10_09__
#define __EXTRACT_H_2011_10_09__

#include "record.h"

#ifdef  __cplusplus
extern "C" {
#endif

extern TBacktraceRecord ExtractBacktraceRecord(void* frame);

#ifdef  __cplusplus
}
#endif

#endif

