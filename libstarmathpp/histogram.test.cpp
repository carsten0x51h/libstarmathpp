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
#define BOOST_TEST_MODULE "histogram unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/histogram.hpp>

using namespace starmathpp;

/**
 *
 */
BOOST_AUTO_TEST_SUITE (histogram_tests)


/**
 *
 */
BOOST_AUTO_TEST_CASE(histogram_black_image_test) {
  Image input_image(5, 5, 1, 1, 0);  // 5x5 - bg value 0

  Histogram h1(input_image, 4);

  BOOST_TEST(h1.get_num_bins() == 4);
  BOOST_TEST(h1.get_lower_boundary() == 0);
  BOOST_TEST(h1.get_upper_boundary() == 0);
  BOOST_TEST(h1.get_value(0) == 25);
}

/**
 *
 */
BOOST_AUTO_TEST_CASE(histogram_5_values_test) {
  Image input_image(5, 5, 1, 1, 0);  // 5x5 - bg value 0
  input_image(0,0) = 10;
  input_image(0,1) = 20;
  input_image(0,2) = 30;
  input_image(0,3) = 30;


  Histogram h1(input_image, 4);

  BOOST_TEST(h1.get_num_bins() == 4);
  BOOST_TEST(h1.get_lower_boundary() == 0);
  BOOST_TEST(h1.get_upper_boundary() == 30);

  BOOST_TEST(h1.get_value(0) == 21);
  BOOST_TEST(h1.get_value(1) == 1);
  BOOST_TEST(h1.get_value(2) == 1);
  BOOST_TEST(h1.get_value(3) == 2);
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(histogram_float_test) {
  Image input_image(5, 5, 1, 1, 0.0F);  // 5x5 - bg value 0
  input_image(0,0) = 10.1F;
  input_image(0,1) = 20.2F;
  input_image(0,2) = 30.3F;
  input_image(0,3) = 70000.0F;


  Histogram h1(input_image, 4);

  BOOST_TEST(h1.get_num_bins() == 4);
  BOOST_TEST(h1.get_lower_boundary() == 0);
  BOOST_TEST(h1.get_upper_boundary() == 70000);

  BOOST_TEST(h1.get_value(0) == 24);
  BOOST_TEST(h1.get_value(3) == 1);
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(histogram_lower_upper_bounds_test) {
  Image input_image(5, 5, 1, 1, 0);  // 5x5 - bg value 0
  input_image(0,0) = 10;
  input_image(0,1) = 50;
  input_image(0,2) = 70;
  input_image(0,3) = 99;

  Histogram h1(input_image, 0.0F /*min pixel*/, 99.0F /*max pixel*/, 4);

  BOOST_TEST(h1.get_num_bins() == 4);
  BOOST_TEST(h1.get_lower_boundary() == 0);
  BOOST_TEST(h1.get_upper_boundary() == 99);

  BOOST_TEST(h1.get_value(0) == 22); // 0, 10
  BOOST_TEST(h1.get_value(1) == 0);
  BOOST_TEST(h1.get_value(2) == 2);  // 50, 70
  BOOST_TEST(h1.get_value(3) == 1);  // 99
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(histogram_invalid_number_of_bins_exception_test) {
  Image input_image(5, 5, 1, 1, 0);  // 5x5 - bg value 0
  BOOST_CHECK_THROW(Histogram h1(input_image, 0), HistogramException);
}

// TODO: Add further tests...

BOOST_AUTO_TEST_SUITE_END();
