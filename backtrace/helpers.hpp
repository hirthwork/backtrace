/*
 * helpers.hpp              -- memory deallocating helpers
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

#ifndef __HELPERS_HPP_2011_09_20__
#define __HELPERS_HPP_2011_09_20__

#include <cstdlib>

#include "record.h"

class TMalloced
{
    void* Data_;

public:
    explicit inline TMalloced(void* data)
        : Data_(data)
    {
    }

    inline ~TMalloced()
    {
        free(Data_);
    }

    inline void* Data() const
    {
        return Data_;
    }

    inline void* Release()
    {
        void* result = Data_;
        Data_ = NULL;
        return result;
    }
};

#endif

