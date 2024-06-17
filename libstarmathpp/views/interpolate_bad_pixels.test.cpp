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
#define BOOST_TEST_MODULE "pipeline view interpolate bad pixels unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/views/interpolate_bad_pixels.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_interpolate_bad_pixels_tests)

using namespace starmathpp;
using namespace ranges;

/**
 * This function generates a CImg
 */
static ImagePtr generate_test_image(unsigned int width, unsigned int height,
                                    unsigned int bad_pixel_pos_x,
                                    unsigned int bad_pixel_pos_y,
                                    float bg_pixel_value,
                                    float bad_pixel_value) {

  auto bad_pixel_image_ptr = std::make_shared < Image
      > (width, height, 1, 1, bg_pixel_value);

  (*bad_pixel_image_ptr)(bad_pixel_pos_x, bad_pixel_pos_y) = bad_pixel_value;

  return bad_pixel_image_ptr;
}

/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_interpolate_bad_test)
{
  std::vector<ImagePtr> input_images = {
    generate_test_image(25, 25, 10, 10, 100, 10000),  // Hot pixel at 10,10
    generate_test_image(25, 25, 10, 10, 10000, 100),// Cold pixel at 10,10
    generate_test_image(25, 25, 10, 10, 100, 115)// Normal pixel at 10,10
  };

  std::vector<Image> expected_result_images = { *generate_test_image(25, 25,
                                                                       10, 10,
                                                                       100,
                                                                       100),  // Hot pixel corrected at 10,10
  *generate_test_image(25, 25, 10, 10, 10000, 10000),  // Cold pixel corrected at 10,10
  *generate_test_image(25, 25, 10, 10, 100, 115)  // Normal pixel not corrected at 10,10
      };

  auto result_images =
      input_images
          | starmathpp::pipeline::views::interpolate_bad_pixels(
              500 /*absolute threshold*/,
              3 /*filter core size*/,
              starmathpp::algorithm::BadPixelMedianInterpolator::ThresholdDirection::BOTH)
          | ranges::views::transform([](const auto &img_ptr) {
            return *img_ptr;
          }) | to<std::vector>();

  BOOST_TEST(result_images.size() == 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
      expected_result_images.begin(), expected_result_images.end());
}

BOOST_AUTO_TEST_SUITE_END();
