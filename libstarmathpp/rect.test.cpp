/*****************************************************************************
 *
 *  libstarmathpp - A C++ library to process astronomical images
 *                  based on CImg and range-v3.
 *
 *  Copyright(C) 2023 Carsten Schmitt <c [at] lost-infinity.com>
 *
 *  More info on https://www.lost-infinity.com
 *
 *  This program is free software ; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation ; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program ; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 ****************************************************************************/

// Shared lib
// This is much faster than the header only variant
#define BOOST_TEST_MODULE "rect unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/rect.hpp>

BOOST_AUTO_TEST_SUITE(rect_tests)

/**
 * Test if operator==() and operator!=() for
 * Rect<int> works as expected.
 */
BOOST_AUTO_TEST_CASE(rect_int_equal_test) {
  Rect<int> r1(-10, 15, 100, 120);
  Rect<int> r2(-10, 15, 100, 120);
  Rect<int> r3(-10, 15, 100, 123);

  BOOST_TEST((r1 == r2) == true);
  BOOST_TEST((r2 == r3) == false);
  BOOST_TEST((r1 != r2) == false);
  BOOST_TEST((r2 != r3) == true);
}

/**
 * Test if operator==() and operator!=() for
 * Rect<unsigned int> works as expected.
 */
BOOST_AUTO_TEST_CASE(rect_unsigned_int_equal_test) {
  Rect<unsigned int> r1(10, 15, 100, 120);
  Rect<unsigned int> r2(10, 15, 100, 120);
  Rect<unsigned int> r3(10, 15, 100, 123);

  BOOST_TEST((r1 == r2) == true);
  BOOST_TEST((r2 == r3) == false);
  BOOST_TEST((r1 != r2) == false);
  BOOST_TEST((r2 != r3) == true);
}

/**
 * Test if operator==() and operator!=() for
 * Rect<float> works as expected. This uses
 * a template specialization to allow for small
 * tolerances.
 */
BOOST_AUTO_TEST_CASE(rect_float_equal_test) {
  Rect<float> r1(10.2F, 15.1F, 100.01F, 120.05F);
  Rect<float> r2(10.2F, 15.1F, 100.01F, 120.05F);
  Rect<float> r3(10.2F, 15.1F, 100.01F, 123.05F);

  BOOST_TEST((r1 == r2) == true);
  BOOST_TEST((r2 == r3) == false);
  BOOST_TEST((r1 != r2) == false);
  BOOST_TEST((r2 != r3) == true);
}


/**
 * Test if operator==() and operator!=() for
 * Rect<double> works as expected. This uses
 * a template specialization to allow for small
 * tolerances.
 */
BOOST_AUTO_TEST_CASE(rect_double_equal_test) {
  Rect<double> r1(10.2, 15.1, 100.01, 120.05);
  Rect<double> r2(10.2, 15.1, 100.01, 120.05);
  Rect<double> r3(10.2, 15.1, 100.01, 123.05);

  BOOST_TEST((r1 == r2) == true);
  BOOST_TEST((r2 == r3) == false);
  BOOST_TEST((r1 != r2) == false);
  BOOST_TEST((r2 != r3) == true);
}

BOOST_AUTO_TEST_SUITE_END();
