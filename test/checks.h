/*
 * checks.h                 -- pure C tests helper macros
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

#ifndef __CHECKS_H_2012_01_12__
#define __CHECKS_H_2012_01_12__

#include <stdio.h>
#include <string.h>

#define REQUIRE_EQUAL(first, second)                        \
    do { if (first != second) {                             \
        printf("%s:%d: assertion failed \"%d\" != \"%d\"\n",\
            __FILE__, __LINE__, (int)first, second);        \
        return 1; } } while (0)

#define REQUIRE_NOT_EQUAL(first, second)                    \
    do { if (first == second) {                             \
        printf("%s:%d: assertion failed \"%d\" == \"%d\"\n",\
            __FILE__, __LINE__, (int)first, second);        \
        return 1; } } while (0)

#define REQUIRE_STRINGS_EQUAL(first, second)                \
    do { if ((void*)first != (void*)second &&               \
        (!first || !second || strcmp(first, second))) {     \
        printf("%s:%d: assertion failed \"%s\" != \"%s\"\n",\
            __FILE__, __LINE__, first, second);             \
        return 1; } } while (0)

#define TO_STRING_IMPL(arg) #arg
#define TO_STRING(arg) TO_STRING_IMPL(arg)

#endif

