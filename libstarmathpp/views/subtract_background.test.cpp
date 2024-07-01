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
#define BOOST_TEST_MODULE "pipeline view subtract background unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/move.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/views/subtract_background.hpp>
#include <libstarmathpp/algorithm/threshold/otsu_thresholder.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_subtract_background_tests)

using namespace starmathpp;
using namespace ranges;

/**
 * This function generates a CImg
 */
static Image generate_test_image(unsigned int width, unsigned int height,
                                 unsigned int test_pixel_pos_x,
                                 unsigned int test_pixel_pos_y,
                                 float bg_pixel_value, float test_pixel_value) {

  Image test_pixel_image(width, height, 1, 1, bg_pixel_value);

  test_pixel_image(test_pixel_pos_x, test_pixel_pos_y) = test_pixel_value;

  return test_pixel_image;
}

/**
 * Test if subtracting the background from an image results in
 * the expected result.
 */
BOOST_AUTO_TEST_CASE(pipeline_subtract_background_image_test) {

  Image test_image_with_one_cold_pixel = generate_test_image(25, 25, 10, 10, 100, 1000);  // 25x25, bg=100, bright pixel in the center
  test_image_with_one_cold_pixel(0, 0) = 0;// Dark pixel in the corner

  std::vector<Image> input_images = { generate_test_image(25, 25, 10, 10, 100,
                                                          1000),  // 25x25, bg=100, brighter pixel at 10,10
  generate_test_image(25, 25, 10, 10, 0, 100),  // 25x25, bg=0, brighter pixel at 10,10
  test_image_with_one_cold_pixel };

  auto result_images = input_images | ranges::views::move
      | starmathpp::pipeline::views::subtract_background(
          starmathpp::algorithm::OtsuThresholder<float>(16))
      | to<std::vector>();

  std::vector<Image> expected_result_images = { generate_test_image(25, 25, 10,
                                                                    10, 0,
                                                                    450.5),  // threshold=549.5 -> 1000-549.5 = 450.5
  generate_test_image(25, 25, 10, 10, 0, 50.5),  // threshold=49.5 -> 100-49.5 = 50.5
  generate_test_image(25, 25, 10, 10, 0, 450.5)  // threshold=549.5 -> 1000-549.5 = 450.5
      };

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

BOOST_AUTO_TEST_SUITE_END();
