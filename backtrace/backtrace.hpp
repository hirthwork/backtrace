/*
 * backtrace.hpp            -- backtrace helper
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

#ifndef __BACKTRACE_HPP_2011_09_20__
#define __BACKTRACE_HPP_2011_09_20__

#include <vector>

#include "record.h"

namespace NBacktrace
{
    struct TBacktrace: std::vector<TBacktraceRecord>
    {
        typedef std::vector<TBacktraceRecord> TBase;
        inline explicit TBacktrace(TBase::size_type n)
            : TBase(n)
        {
        }
    };

    TBacktrace GetBacktrace(int offset = 0, int initialDepth = 10);
}

#endif

