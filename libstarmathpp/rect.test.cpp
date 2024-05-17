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


/**
 * Test the rect contains() function for int.
 * NOTE: This is the "opposite" of the inside().
 */
BOOST_AUTO_TEST_CASE(rect_int_contains_test) {
  Rect<int> r1(4, 5, 10, 15);
  BOOST_TEST(r1.contains(r1) == true);

  Rect<int> r2(-4, -5, 10, 15);
  BOOST_TEST(r2.contains(r2) == true);

  Rect<int> r3(4, 5, 0, 0);
  BOOST_TEST(r3.contains(r3) == true);

  Rect<int> r4;
  BOOST_TEST(r4.inside(r4) == false);


  Rect<int> r5(4, 5, 10, 15);
  Rect<int> r6(5, 6, 7, 9);

  BOOST_TEST(r6.contains(r5) == false);
  BOOST_TEST(r5.contains(r6) == true);

  Rect<int> r7(-4, -5, 10, 15);
  Rect<int> r8(-3, -4, 7, 9);

  BOOST_TEST(r8.contains(r7) == false);
  BOOST_TEST(r7.contains(r8) == true);


  BOOST_TEST(Rect<int>(4, 5, 10, 15).contains(Rect<int>(4, 5, 11, 15)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 15).contains(Rect<int>(4, 5, 10, 16)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 15).contains(Rect<int>(3, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 15).contains(Rect<int>(4, 4, 10, 15)) == false);

  BOOST_TEST(Rect<int>(5, 6, 0, 0).contains(Rect<int>(4, 5, 10, 15)) == false);

  BOOST_TEST(Rect<int>().contains(Rect<int>(4, 5, 10, 15)) == false);
  BOOST_TEST(Rect<int>(4, 5, 10, 15).contains(Rect<int>()) == false);
}


/**
 * Test the rect grow() function.
 */
BOOST_AUTO_TEST_CASE(rect_grow_test) {

  BOOST_TEST(Rect<int>(4, 5, 10, 15).grow(2) == Rect<int>(2, 3, 14, 19));
  BOOST_TEST(Rect<int>(0, 0, 10, 15).grow(2) == Rect<int>(-2, -2, 14, 19));

  BOOST_TEST(Rect<int>(0, 0, 10, 15).grow(1.4F) == Rect<float>(-1.4F, -1.4F, 12.8F, 17.8F));
  BOOST_TEST(Rect<int>(0, 0, 10, 15).grow(1.6) == Rect<double>(-1.6, -1.6, 13.2, 18.2));

  BOOST_TEST(Rect<float>(4.0F, 5.0F, 10.0F, 15.0F).grow(1.0F) == Rect<float>(3.0F, 4.0F, 12.0F, 17.0F));
  BOOST_TEST(Rect<double>(4.0, 5.0, 10.0, 15.0).grow(1.0) == Rect<double>(3.0, 4.0, 12.0, 17.0));
  BOOST_TEST(Rect<float>(4.0F, 5.0F, 10.0F, 15.0F).grow(1) == Rect<int>(3, 4, 12, 17));

  BOOST_TEST(Rect<unsigned int>(4, 5, 10, 15).grow(2u) == Rect<unsigned int>(2, 3, 14, 19));

  // grow() on uninitialized Rect should fail with RectException.
  Rect<int> unset_rect;
  BOOST_CHECK_THROW(unset_rect.grow(2), RectException);
}


/**
 * Test the rect shrink() function.
 */
BOOST_AUTO_TEST_CASE(rect_shrink_test) {
  BOOST_TEST(Rect<int>(4, 5, 10, 15).shrink(2) == Rect<int>(6, 7, 6, 11));
  BOOST_TEST(Rect<int>(-2, -3, 10, 15).shrink(2) == Rect<int>(0, -1, 6, 11));

  BOOST_TEST(Rect<int>(-2, -3, 10, 15).shrink(1.4F) == Rect<float>(-0.6F, -1.6F, 7.2F, 12.2F));
  BOOST_TEST(Rect<double>(-3.2, -3.2, 10, 15).shrink(1.6) == Rect<double>(-1.6, -1.6, 6.8, 11.8));

  // TODO: Add further unit tests (see grow())

  // shrink() on uninitialized Rect should fail with RectException.
  Rect<int> unset_rect;
  BOOST_CHECK_THROW(unset_rect.shrink(2), RectException);
}



/**
 * Test the rect expand_to_square() function.
 */
BOOST_AUTO_TEST_CASE(rect_expand_to_square_test) {
  BOOST_TEST(Rect<int>(4, 5, 10, 15).expand_to_square() == Rect<int>(1, 5, 15, 15));
  BOOST_TEST(Rect<float>(4.0F, 5.0F, 10.0F, 15.0F).expand_to_square() == Rect<float>(1.5F, 5.0F, 15.0F, 15.0F));

  BOOST_TEST(Rect<int>(4, 5, 15, 10).expand_to_square() == Rect<int>(4, 2, 15, 15));
  BOOST_TEST(Rect<float>(4.0F, 5.0F, 15.0F, 10.0F).expand_to_square() == Rect<float>(4.0F, 2.5F, 15.0F, 15.0F));

  // TODO: Add more unit tests
}


/**
 * Test the rect center() function.
 */
BOOST_AUTO_TEST_CASE(rect_center_test) {
  BOOST_TEST(Rect<int>(4, 5, 10, 15).center<float>() == Point<float>(9, 12.5));
  BOOST_TEST(Rect<int>(4, 5, 10, 15).center() == Point<int>(9, 12));

  // TODO: Add more unit tests
}


/**
 * Test the rect from_center_point() function.
 */
BOOST_AUTO_TEST_CASE(rect_from_center_point_test) {
  BOOST_TEST(Rect<int>::from_center_point(Point(5, 6), 8, 10) == Rect<int>(1, 1, 8, 10));

  // TODO: Add more unit tests
}


BOOST_AUTO_TEST_SUITE_END();

// TODO: namespace!
