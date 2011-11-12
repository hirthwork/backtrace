/*
 * test-demangle.cpp        -- demangle routine tests
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

#include <demangle.hpp>

#define BOOST_TEST_MODULE DemangleTest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(demangle)
{
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle(
        "_ZN10test_frame11test_methodEv"), "test_frame::test_method()");
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle(
        "_ZN17NReinventedWheels15GetCurrentFrameEjb"),
        "NReinventedWheels::GetCurrentFrame(unsigned int, bool)");
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle("GetCurrentFrame"),
        "GetCurrentFrame");
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle("_Z1fv"), "f()");
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle(""), "");
    BOOST_REQUIRE_EQUAL(NReinventedWheels::Demangle(NULL), "");
}

