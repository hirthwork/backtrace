/*
 * record.hpp               -- demangled backtrace information atom
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

#ifndef __RECORD_HPP_2011_09_20__
#define __RECORD_HPP_2011_09_20__

#include <string>

#include "record.h"

#include "demangle.hpp"

namespace NBacktrace
{
    struct TDemangledBacktraceRecord
    {
        // Frame pointer, always valid and can be passed to addr2line
        const void* const Frame_;
        // NULL indicates error during record extraction
        const char* const Module_;
        // Address where module is loaded
        const void* const ModuleAddress_;
        // NULL indicates symbol information absence
        const char* const Symbol_;
        // Symbol address, can be used for offset calculation
        const void* const SymbolAddress_;
        // same as Symbol_ if demangling failed or empty string if no symbol
        // information available
        const std::string Function_;

        explicit inline TDemangledBacktraceRecord(
            const TBacktraceRecord& record)
            : Frame_(record.Frame_)
            , Module_(record.Module_)
            , ModuleAddress_(record.ModuleAddress_)
            , Symbol_(record.Symbol_)
            , SymbolAddress_(record.SymbolAddress_)
            , Function_(Demangle(Symbol_))
        {
        }
    };
}

#endif

