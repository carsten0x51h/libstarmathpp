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

#include <vector>
#include <tuple>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <libstarmathpp/algorithm/bad_pixel_median_interpolator.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_bad_pixel_median_interpolator_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;
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
BOOST_DATA_TEST_CASE(absolute_threshold_test,
    bdata::make(
        // bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value, absolute_detection_threshold, expected_pixel_value, threshold_direction
        std::vector< std::tuple<unsigned int, unsigned int, float, float, float, float, typename BadPixelMedianInterpolator::ThresholdDirection::TypeE> > {
          // Check if the hot pixel is interpolation correctly
          { 10, 10, 100, 10000, 500.0F, 100, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check if the cold pixel is interpolation correctly
          { 10, 10, 10000, 100, 500.0F, 10000, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check if a bad pixel is interpolation correctly at the border
          { 0, 3, 10000, 100, 500.0F, 10000, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check if a bad pixel is interpolation correctly at the corner
          { 24, 24, 10000, 100, 500.0F, 10000, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check if the interpolator includes values >= threshold
          { 10, 10, 100, 600, 500.0F, 100, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check if bad pixel remains untouched if threshold is not reached
          { 10, 10, 100, 599, 500.0F, 599, BadPixelMedianInterpolator::ThresholdDirection::BOTH},

          // Check that bad pixel interpolator with positive threshold direction corrects hot pixels
          { 10, 10, 100, 10000, 500.0F, 100, BadPixelMedianInterpolator::ThresholdDirection::POSITIVE},

          // Check that bad pixel interpolator with positive threshold direction does not correct cold pixels
          { 10, 10, 10000, 100, 500.0F, 100, BadPixelMedianInterpolator::ThresholdDirection::POSITIVE},

          // Check that bad pixel interpolator with negative threshold direction corrects cold pixels
          { 10, 10, 10000, 100, 500.0F, 10000, BadPixelMedianInterpolator::ThresholdDirection::NEGATIVE},

          // Check that bad pixel interpolator with negative threshold direction does not correct hot pixels
          { 10, 10, 100, 10000, 500.0F, 10000, BadPixelMedianInterpolator::ThresholdDirection::NEGATIVE}
        }) *
    bdata::make(
        std::vector<unsigned int> {
          3,5,7,9
        }),
    bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value, absolute_detection_threshold, expected_pixel_value, threshold_direction, filter_core_size)
{
  BadPixelMedianInterpolator bad_pixel_median_interpolator(
      absolute_detection_threshold,
      filter_core_size,
      threshold_direction);

  // Check, if the bad pixel was correctly interpolated with the median background value.
  BOOST_TEST(
      (*bad_pixel_median_interpolator.interpolate(
              generate_bad_pixel_image(
                  25, 25, bad_pixel_pos_x, bad_pixel_pos_y, bg_pixel_value, bad_pixel_value)
          )
      )(bad_pixel_pos_x, bad_pixel_pos_y) == expected_pixel_value
  );

}

/**
 *
 */
BOOST_AUTO_TEST_CASE(invalid_parameters_exception_test)
{
  // Check if BadPixelMedianInterpolator throws if invalid
  // absolute threshold value is passed as parameter.
  BOOST_CHECK_THROW(
    BadPixelMedianInterpolator(
        -500 /*invalid*/,
        3,
        BadPixelMedianInterpolator::ThresholdDirection::BOTH),
    BadPixelMedianInterpolatorException);


  // Check if BadPixelMedianInterpolator throws if unsupported filter
  // core size is passed as parameter.
  BOOST_CHECK_THROW(
    BadPixelMedianInterpolator(
        500,
        4 /*invalid*/,
        BadPixelMedianInterpolator::ThresholdDirection::BOTH),
    BadPixelMedianInterpolatorException);


  // Check if BadPixelMedianInterpolator throws if an invalid
  // threshold direction is passed as parameter.
  BOOST_CHECK_THROW(
    BadPixelMedianInterpolator(
        500,
        3,
        BadPixelMedianInterpolator::ThresholdDirection::_Count /*invalid*/),
    BadPixelMedianInterpolatorException);
}

BOOST_AUTO_TEST_SUITE_END();
