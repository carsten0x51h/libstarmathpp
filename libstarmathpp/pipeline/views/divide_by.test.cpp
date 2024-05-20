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
#define BOOST_TEST_MODULE "pipeline view divide_by unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/pipeline/views/divide_by.hpp>
#include <libstarmathpp/inconsistent_image_dimensions_exception.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_divide_by_tests)

using namespace starmathpp;
using namespace ranges;

/**
 * Test if dividing an image by another image results in the
 * expected pixel values.
 */
BOOST_AUTO_TEST_CASE(pipeline_divide_by_image_test)
{
  std::vector<ImagePtr> input_images = {
    std::make_shared<Image>(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
    std::make_shared<Image>(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
    std::make_shared<Image>(5, 5, 1, 1, -10)// 5x5 - All pixels have value -10
  };

  std::vector<Image> expected_result_images = { Image(5, 5, 1, 1, 6.5F),  // 5x5 - All pixels have value 6.5F
  Image(5, 5, 1, 1, 5),  // 5x5 - All pixels have value 5
  Image(5, 5, 1, 1, -5)  // 5x5 - All pixels have value -5
      };

  auto divisor_image_5x5_value2_ptr1 = std::make_shared < Image
      > (5, 5, 1, 1, 2);

  // NOTE: | views::indirect causes a segmentation fault
  auto result_images = input_images
      | pipeline::views::divide_by(divisor_image_5x5_value2_ptr1)
      | views::transform([](const auto &img_ptr) {
        return *img_ptr;
      }) | to<std::vector>();

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

/**
 * Test if dividing an image by a scalar value works.
 */
BOOST_AUTO_TEST_CASE(pipeline_divide_by_scalar_test)
{
  std::vector<ImagePtr> input_images = {
    std::make_shared<Image>(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
    std::make_shared<Image>(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
    std::make_shared<Image>(5, 5, 1, 1, -10)// 5x5 - All pixels have value -10
  };

  std::vector<Image> expected_result_images = { Image(5, 5, 1, 1, 6.5F),  // 5x5 - All pixels have value 513
  Image(5, 5, 1, 1, 5),  // 5x5 - All pixels have value 510
  Image(5, 5, 1, 1, -5)  // 5x5 - All pixels have value 510
      };

  auto result_images = input_images | pipeline::views::divide_by(2.0F)
      | views::transform([](const auto &img_ptr) {
        return *img_ptr;
      }) | to<std::vector>();

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

/**
 * Test if dividing two images of different sizes fails with an exception.
 */
BOOST_AUTO_TEST_CASE(pipeline_divide_by_different_image_sizes_test)
{
  auto image_5x5_value9_ptr1 = std::make_shared<Image>(5, 5, 1, 1, 9);
  auto image_4x4_value9_ptr2 = std::make_shared < Image
      > (4, 4, 1, 1, 9);

  BOOST_CHECK_THROW(ranges::views::single(image_5x5_value9_ptr1)
      | pipeline::views::divide_by(image_4x4_value9_ptr2)
      | to<std::vector>(),
      starmathpp::InconsistentImageDimensionsException);
}

BOOST_AUTO_TEST_SUITE_END();
