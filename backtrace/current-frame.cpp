/*
 * current-frame.cpp        -- current frame retrieval implementation
 *
 * Copyright (C) 2012 Dmitry Potapov <potapov.d@gmail.com>
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

#include <stdexcept>
// this include required for std::logic_error construction, as it accepts
// const reference to std::string
#include <string>

#include "extract.h"
#include "frames.h"
#include "record.h"

#include "current-frame.hpp"

TBacktraceRecord NBacktrace::GetCurrentFrame(int offset)
{
    if (void* frame = GetFrame(offset + 1))
    {
        return ExtractBacktraceRecord(frame);
    }
    throw std::logic_error("offset is bigger than call stack");
}

