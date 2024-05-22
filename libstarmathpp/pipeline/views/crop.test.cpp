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
#define BOOST_TEST_MODULE "pipeline view crop unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <memory>

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>

#include <libstarmathpp/pipeline/views/crop.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_crop_tests)

using namespace starmathpp;
using namespace starmathpp::pipeline;
using namespace starmathpp::pipeline::views;
using namespace ranges;

/**
 * This function generates a test-image.
 *
 * TODO: Add crop_from_center test for even sized regions.
 * TODO: Test specified region exceeding the image dimensions.
 */
static ImagePtr generate_test_image(unsigned int width, unsigned int height,
                                    unsigned int test_pixel_pos_x,
                                    unsigned int test_pixel_pos_y,
                                    float bg_pixel_value,
                                    float test_pixel_value) {

  auto test_pixel_image_ptr = std::make_shared<Image>(width, height, 1, 1,
                                                      bg_pixel_value);

  (*test_pixel_image_ptr)(test_pixel_pos_x, test_pixel_pos_y) =
      test_pixel_value;

  return test_pixel_image_ptr;
}

/**
 * Test cropping a 3x3 rectangle from a 25x25 image with a
 * bright pixel in the center.
 */
BOOST_AUTO_TEST_CASE(pipeline_crop_from_center_sub_region_test)
{
  auto input_image = generate_test_image(25, 25, 12, 12, 250, 65535.0F);
  auto expected_result_image = generate_test_image(3, 3, 1, 1, 250, 65535.0F);

  auto result_images = view::single(input_image)
      | crop_from_center(Size<int>(3, 3)) | to<std::vector>();

  // NOTE: Exactly one image is expected
  BOOST_TEST(result_images.size() == 1);
  BOOST_TEST(is_almost_equal(*(result_images.at(0)),*expected_result_image, 0.00001));
}

/**
 * Test cropping a 5x5 rectangle from a 5x5 image with a
 * bright pixel in the center.
 */
BOOST_AUTO_TEST_CASE(pipeline_crop_from_center_full_image_test)
{
  auto input_image = generate_test_image(5, 5, 2, 2, 250, 65535.0F);
  auto result_images = view::single(input_image)
      | crop_from_center(Size<int>(5, 5)) | to<std::vector>();

  // NOTE: Exactly one image is expected
  BOOST_TEST(result_images.size() == 1);
  BOOST_TEST(is_almost_equal(*(result_images.at(0)), *input_image, 0.00001));
}

/**
 * range<image>   -->  crop(rects)  --> range < range <image> >
 *       ^                                          ^
 *       |                                          |
 *  input image                         range of cropped images
 *
 *  Per input image, N cropped images (one per rect).
 */
BOOST_AUTO_TEST_CASE(pipeline_multi_crop_on_image_test)
{
  auto input_image = generate_test_image(25, 25, 5, 5, 250, 65535.0F);

  std::vector<Rect<int>> rects = { Rect<int>(0, 0, 9, 9), Rect<int>(11, 11, 9,
                                                                    9) };
  auto expected_result_image_1 = generate_test_image(9, 9, 5, 5, 250, 65535.0F);
  auto expected_result_image_2 = generate_test_image(9, 9, 5, 5, 250, 250);

  auto cropped_images = view::single(input_image) | crop(rects)
      | to<std::vector>();

  BOOST_TEST(cropped_images.size() == 1);  // One image goes in, one result is produced (which wraps a vector)
  BOOST_TEST(cropped_images.at(0).size() == 2);// Two rects as input produce two output images
  BOOST_TEST(is_almost_equal(*(cropped_images.at(0).at(0)), *expected_result_image_1, 0.00001));
  BOOST_TEST(is_almost_equal(*(cropped_images.at(0).at(1)), *expected_result_image_2, 0.00001));
}

/**
 * range<image>   -->  crop(rect)  --> range <image>
 *        ^                                     ^
 *        |                                     |
 *   input image                          cropped image
 *
 * Per input image, one cropped image.
 */
BOOST_AUTO_TEST_CASE(pipeline_crop_on_image_test)
{
  auto rect = Rect<int>(5, 5, 10, 10);

  std::vector<ImagePtr> input_images = { generate_test_image(25, 25, 8, 8, 250,
                                                             65535.0F),
      generate_test_image(25, 25, 10, 10, 250, 65535.0F), generate_test_image(
          25, 25, 12, 12, 250, 65535.0F) };

  auto expected_result_image_1 = generate_test_image(10, 10, 3, 3, 250,
                                                     65535.0F);
  auto expected_result_image_2 = generate_test_image(10, 10, 5, 5, 250,
                                                     65535.0F);
  auto expected_result_image_3 = generate_test_image(10, 10, 7, 7, 250,
                                                     65535.0F);

  auto cropped_images = input_images | crop(rect) | to<std::vector>();

  BOOST_TEST(cropped_images.size() == 3);
  BOOST_TEST(is_almost_equal(*(cropped_images.at(0)), *expected_result_image_1, 0.00001));
  BOOST_TEST(is_almost_equal(*(cropped_images.at(1)), *expected_result_image_2, 0.00001));
  BOOST_TEST(is_almost_equal(*(cropped_images.at(2)), *expected_result_image_3, 0.00001));
}

BOOST_AUTO_TEST_SUITE_END();
