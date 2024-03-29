/*
 * test-backtrace-c.c       -- backtrace functions tests
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

#include <errno.h>
#include <stdlib.h>

#include <backtrace/backtrace.h>
#include <backtrace/record.h>

#include "checks.h"

static int TestGetCurrentFrameStatic()
{
    struct TBacktraceRecord record;
    int result = GetCurrentFrame(&record, 0);
    REQUIRE_EQUAL(result, 1);
#ifdef __FreeBSD__
    REQUIRE_STRINGS_EQUAL(record.Symbol_, "_start");
#else
    REQUIRE_EQUAL(record.Symbol_, 0);
#endif
    return 0;
}

int TestGetCurrentFrame()
{
    struct TBacktraceRecord record;
    int result = GetCurrentFrame(&record, 0);
    REQUIRE_EQUAL(result, 1);
    REQUIRE_STRINGS_EQUAL(record.Symbol_, "TestGetCurrentFrame");
    result = GetCurrentFrame(&record, 1);
    REQUIRE_EQUAL(result, 1);
    REQUIRE_STRINGS_EQUAL(record.Symbol_, "main");
    result = GetCurrentFrame(&record, 100);
    REQUIRE_EQUAL(result, 0);
    return 0;
}

int TestGetBacktrace()
{
    int size;
    int sizeMinus1;
    struct TBacktraceRecord* backtrace = GetBacktrace(&size, 0, 1);
    REQUIRE_NOT_EQUAL(backtrace, 0);
    REQUIRE_STRINGS_EQUAL(backtrace->Symbol_, "TestGetBacktrace");
    REQUIRE_STRINGS_EQUAL(backtrace[1].Symbol_, "main");
    free(backtrace);
    backtrace = GetBacktrace(&sizeMinus1, 1, 10);
    REQUIRE_NOT_EQUAL(backtrace, 0);
    REQUIRE_EQUAL(size - 1, sizeMinus1);
    REQUIRE_STRINGS_EQUAL(backtrace->Symbol_, "main");
    free(backtrace);
    backtrace = GetBacktrace(&size, 100, 10);
    REQUIRE_EQUAL(backtrace, 0);
    REQUIRE_EQUAL(errno, ERANGE);
    backtrace = GetBacktrace(&size, 100, 0x7fffffff);
    REQUIRE_EQUAL(backtrace, 0);
    REQUIRE_EQUAL(errno, EDOM);
    return 0;
}

int main()
{
    return TestGetCurrentFrame() + TestGetCurrentFrameStatic()
        + TestGetBacktrace();
}

