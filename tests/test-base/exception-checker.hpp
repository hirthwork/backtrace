/*
 * exception-checker.hpp    -- helper class for exception checking
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

#ifndef __EXCEPTION_CHECKER_HPP_2012_01_21__
#define __EXCEPTION_CHECKER_HPP_2012_01_21__

#include <cstring>

#include <exception>

class TExceptionChecker
{
    const char* const Text_;

public:
    inline TExceptionChecker(const char* text)
        : Text_(text)
    {
    }

    inline bool operator () (const std::exception& exc) const
    {
        return strcmp(exc.what(), Text_) == 0;
    }
};

#endif

