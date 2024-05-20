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
#define BOOST_TEST_MODULE "algorithm bad pixel median interpolator unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <libstarmathpp/algorithm/bad_pixel_median_interpolator.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_bad_pixel_median_interpolator_tests)

using namespace starmathpp;
using namespace ranges;

namespace bdata = boost::unit_test::data;

/**
 * This function generates a CImg
 */
static std::shared_ptr<Image> generate_bad_pixel_image(
    unsigned int width, unsigned int height, unsigned int bad_pixel_pos_x,
    unsigned int bad_pixel_pos_y, float bg_pixel_value, float bad_pixel_value) {

  auto bad_pixel_image_ptr = std::make_shared < Image
      > (width, height, 1, 1, bg_pixel_value);

  (*bad_pixel_image_ptr)(bad_pixel_pos_x, bad_pixel_pos_y) = bad_pixel_value;

  return bad_pixel_image_ptr;
}

/**
 *
 */
BOOST_DATA_TEST_CASE(absolute_threshold_both_directions_3x3_test,
    bdata::make(
        std::vector< std::tuple<unsigned int /*width*/, unsigned int /*height*/, unsigned int /*bad_pixel_pos_x*/, unsigned int /*bad_pixel_pos_y*/, float /*bg_pixel_value*/, float /*bad_pixel_value*/> > {
          { 25, 25, 10, 10, 100, 10000}, // Check if the hot pixel is interpolation correctly
          { 25, 25, 10, 10, 10000, 100}, // Check if the cold pixel is interpolation correctly
          { 25, 25, 0, 3, 10000, 100},   // Check if a bad pixel is interpolation correctly at the border
          { 25, 25, 24, 24, 10000, 100}  // Check if a bad pixel is interpolation correctly at the corner
        }),
    width, height, bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value)
{
  using namespace starmathpp::algorithm;

  BadPixelMedianInterpolator bad_pixel_median_interpolator(
      500 /*absolute_detection_threshold*/,
      3 /*filter_core_size*/,
      BadPixelMedianInterpolator::ThresholdDirection::BOTH /*threshold_direction*/);

  generate_bad_pixel_image(
      width, height, bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value);

  // Check, if the bad pixel was correctly interpolated with the median background value.
  BOOST_TEST(
      (*bad_pixel_median_interpolator.interpolate(
              generate_bad_pixel_image(
                  width, height, bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value)
          )
      )(bad_pixel_pos_x, bad_pixel_pos_y) == bg_pixel_value
  );

}

// TODO: dead/cold _only_ pixel
// TODO: Test different thresholds, negative ones?
// TODO: Negative threshold?
// TODO: Negative core size? -> exception

BOOST_AUTO_TEST_SUITE_END();
