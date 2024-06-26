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
#define BOOST_TEST_MODULE "pipeline view add unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/views/add.hpp>
#include <libstarmathpp/inconsistent_image_dimensions_exception.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_add_tests)

using namespace starmathpp;
using namespace ranges;

/**
 * Test if adding an image to another image results in the
 * expected pixel values.
 */
BOOST_AUTO_TEST_CASE(pipeline_add_image_test)
{
  std::vector<Image> input_images = {
    Image(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
    Image(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
    Image(5, 5, 1, 1, -10)// 5x5 - All pixels have value -10
  };

  std::vector<Image> expected_result_images = { Image(5, 5, 1, 1, 22),  // 5x5 - All pixels have value 22
  Image(5, 5, 1, 1, 19),  // 5x5 - All pixels have value 19
  Image(5, 5, 1, 1, -1)  // 5x5 - All pixels have value -1
      };

  Image image_to_add_5x5_value9_ptr1(5, 5, 1, 1, 9);

  // NOTE: | views::indirect causes a segmentation fault
  auto result_images = input_images | ranges::views::move
      | pipeline::views::add(image_to_add_5x5_value9_ptr1) | to<std::vector>();

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

/**
 * Test if adding a scalar to an image works.
 */
BOOST_AUTO_TEST_CASE(pipeline_add_scalar_test)
{
  std::vector<Image> input_images = {
    Image(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
    Image(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
    Image(5, 5, 1, 1, -10)// 5x5 - All pixels have value -10
  };

  std::vector<Image> expected_result_images = { Image(5, 5, 1, 1, 513),  // 5x5 - All pixels have value 513
  Image(5, 5, 1, 1, 510),  // 5x5 - All pixels have value 510
  Image(5, 5, 1, 1, 490)  // 5x5 - All pixels have value 510
      };

  auto result_images = input_images
      | ranges::views::move
      | pipeline::views::add(500.0F)
      | to<std::vector>();

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

/**
 * Test if adding two images of different sizes fails with an exception.
 */
BOOST_AUTO_TEST_CASE(pipeline_add_different_image_sizes_test)
{
  Image image_to_add_5x5_value9_1(5, 5, 1, 1, 9);
  Image image_to_add_4x4_value9_2(4, 4, 1, 1, 9);

  BOOST_CHECK_THROW(
      ranges::views::single(image_to_add_5x5_value9_1)
        | ranges::views::move
        | pipeline::views::add(image_to_add_4x4_value9_2)
        | to<std::vector>(),
      starmathpp::InconsistentImageDimensionsException);
}

BOOST_AUTO_TEST_SUITE_END();
