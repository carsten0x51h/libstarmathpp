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

using namespace starmathpp;

/**
 * TODO: Add tests for:
 *
 *  contains()
 *  center()
 *  expand_to_square()
 *  grow()
 *  shrink()
 *  from_center_point()
 *  to<>()
 */
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


/**
 * Test if operator<<() works correctly.
 */
BOOST_AUTO_TEST_CASE(rect_insertion_operator_test) {
  Rect<int> r1(4, 5, 10, 15);

  std::stringstream ss;
  ss << r1;

  BOOST_TEST(ss.str() == "(x=4, y=5, width=10, height=15)");
}



/**
 * Test set and get functions.
 */
BOOST_AUTO_TEST_CASE(rect_set_and_get_function_test) {
  Rect<int> r1(4, 5, 10, 15);

  BOOST_TEST(r1.x() == 4);
  BOOST_TEST(r1.y() == 5);
  BOOST_TEST(r1.width() == 10);
  BOOST_TEST(r1.height() == 15);

  r1.set_x(6);
  r1.set_y(7);
  r1.set_width(11);
  r1.set_height(16);

  BOOST_TEST(r1.x() == 6);
  BOOST_TEST(r1.y() == 7);
  BOOST_TEST(r1.width() == 11);
  BOOST_TEST(r1.height() == 16);
}


/**
 * Test if the is_set function returns the correct
 * value under different conditions.
 */
BOOST_AUTO_TEST_CASE(rect_is_set_state_test) {
  Rect<int> r1(4, 5, 10, 15);
  BOOST_TEST(r1.is_set() == true);

  Rect<int> r2;
  BOOST_TEST(r2.is_set() == false);
  r2.set_x(3);
  BOOST_TEST(r2.is_set() == true);

  Rect<int> r3;
  BOOST_TEST(r3.is_set() == false);
  r3.set_y(4);
  BOOST_TEST(r3.is_set() == true);

  Rect<int> r4;
  BOOST_TEST(r4.is_set() == false);
  r4.set_width(10);
  BOOST_TEST(r4.is_set() == true);

  Rect<int> r5;
  BOOST_TEST(r5.is_set() == false);
  r5.set_height(15);
  BOOST_TEST(r5.is_set() == true);
}

/**
 * Test if the clear function works as expected.
 */
BOOST_AUTO_TEST_CASE(rect_clear_test) {
  Rect<int> r1(4, 5, 10, 15);
  BOOST_TEST(r1.is_set() == true);

  r1.clear();

  BOOST_TEST(r1.is_set() == false);
  BOOST_TEST(r1.x() == 0);
  BOOST_TEST(r1.y() == 0);
  BOOST_TEST(r1.width() == 0);
  BOOST_TEST(r1.height() == 0);
}


/**
 * Test the rect inside() function for int.
 */
BOOST_AUTO_TEST_CASE(rect_int_inside_test) {
  Rect<int> r1(4, 5, 10, 15);
  BOOST_TEST(r1.inside(r1) == true);

  Rect<int> r2(-4, -5, 10, 15);
  BOOST_TEST(r2.inside(r2) == true);

  Rect<int> r3(4, 5, 0, 0);
  BOOST_TEST(r3.inside(r3) == true);

  Rect<int> r4;
  BOOST_TEST(r4.inside(r4) == false);


  Rect<int> r5(4, 5, 10, 15);
  Rect<int> r6(5, 6, 7, 9);

  BOOST_TEST(r6.inside(r5) == true);
  BOOST_TEST(r5.inside(r6) == false);

  Rect<int> r7(-4, -5, 10, 15);
  Rect<int> r8(-3, -4, 7, 9);

  BOOST_TEST(r8.inside(r7) == true);
  BOOST_TEST(r7.inside(r8) == false);


  BOOST_TEST(Rect<int>(4, 5, 11, 15).inside(Rect<int>(4, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 16).inside(Rect<int>(4, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(3, 5, 10, 15).inside(Rect<int>(4, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(4, 4, 10, 15).inside(Rect<int>(4, 5, 10, 15)) == false);

  BOOST_TEST(Rect<int>(5, 6, 0, 0).inside(Rect<int>(4, 5, 10, 15)) == true);

  BOOST_TEST(Rect<int>().inside(Rect<int>(4, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 15).inside(Rect<int>()) == false);
}




BOOST_AUTO_TEST_SUITE_END();

// TODO: namespace!
