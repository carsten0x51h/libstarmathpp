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
#define BOOST_TEST_MODULE "algorithm average unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/average.hpp>
#include <libstarmathpp/floating_point_equality.hpp>
#include <libstarmathpp/inconsistent_image_dimensions_exception.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_average_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;
using namespace ranges;

/**
 *
 */
BOOST_AUTO_TEST_CASE(algorithm_average_image_calculation_test)
{
  std::vector<Image> input_images;

  input_images.push_back(Image(5, 5, 1, 1, 13));  // 5x5 - All pixels have value 13
  input_images.push_back(Image(5, 5, 1, 1, 10));// 5x5 - All pixels have value 10
  input_images.push_back(Image(5, 5, 1, 1, -5));// 5x5 - All pixels have value -5

  auto average_image = starmathpp::algorithm::average(input_images);
  Image expected_image(5, 5, 1, 1, (13 + 10 - 5) / 3);  // 5x5 - All pixels have value 6

  BOOST_TEST(is_almost_equal(average_image, expected_image, 0.00001));
}

/**
 *
 */
BOOST_AUTO_TEST_CASE(algorithm_average_single_image_test)
{
  std::vector<Image> input_images;
  input_images.push_back(Image(5, 5, 1, 1, 13));  // 5x5 - All pixels have value 13

  auto average_image = starmathpp::algorithm::average(input_images);
  Image expected_image(5, 5, 1, 1, 13);  // 5x5 - All pixels have value 13

  BOOST_TEST(is_almost_equal(average_image, expected_image, 0.00001));
}

/**
 *
 */
BOOST_AUTO_TEST_CASE(algorithm_average_different_image_sizes_exception_test)
{
  std::vector<Image> input_images {
    Image(4, 4, 1, 1, 13),  // 4x4 - All pixels have value 13
    Image(5, 5, 1, 1, 10)   // 5x5 - All pixels have value 10
  };

    BOOST_CHECK_THROW(auto average_image_ptr = starmathpp::algorithm::average(input_images), InconsistentImageDimensionsException);
  }

  /**
   *
   */
  BOOST_AUTO_TEST_CASE(algorithm_average_empty_range_test)
  {
    std::vector<Image> empty_image_vector;
    auto result_image = starmathpp::algorithm::average(empty_image_vector);

    BOOST_CHECK(result_image.is_empty());
  }

  BOOST_AUTO_TEST_SUITE_END();
