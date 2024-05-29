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
#define BOOST_TEST_MODULE "algorithm center of gravity centroider unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/centroid/center_of_gravity_centroider.hpp>
#include <libstarmathpp/floating_point_equality.hpp>
#include <libstarmathpp/image.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_center_of_gravity_centroider_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

namespace bdata = boost::unit_test::data;

/**
 * Check Centroid of empty image. Expectation is that valid()
 * returns false and the optional return value has no value.
 *
 * TODO: Test case where centroider was not able to calculate
 *       a centroid, i.e. returns an "empty" optional...
 */
BOOST_AUTO_TEST_CASE(algorithm_center_of_gravity_centroider_empty_image_test)
{
  Image null_image;

  CenterOfGravityCentroider<float> centroider;

  BOOST_CHECK_THROW(auto centroid = centroider.calculate_centroid(null_image), CentroiderException);
}

/**
 * Calculate centroids using the center of gravity (COG) method.
 * This array contains pairs of filename and expected centroid.
 * The expected values were cross-checked manually.
 */
BOOST_DATA_TEST_CASE(algorithm_center_of_gravity_centroider_test, bdata::make(
        std::vector< std::tuple<std::string, Point<float> > > {
          { "test_data/algorithm/centroid/test_image_all_pixels_value_1_5x5.tiff", Point<float>(2.0F,2.0F)},
          { "test_data/algorithm/centroid/test_image_one_pixel_bottom_right_5x5.tiff", Point<float>(3.0F,3.0F)},
          { "test_data/algorithm/centroid/test_image_one_pixel_top_left_10x10.tiff", Point<float>(1.0F,1.0F)},
          { "test_data/algorithm/centroid/test_image_two_opposite_pixels_5x5.tiff", Point<float>(2.0F,2.0F)},
          { "test_data/algorithm/centroid/test_image_noise_on_the_left_and_one_center_pixel_5x5.tiff", Point<float>(1.88378F,2.0F)}
        }),
    image_filename, expected_centroid_point)
{
  CenterOfGravityCentroider<float> centroider;

  Image test_image(image_filename.c_str());
  auto centroid_opt = centroider.calculate_centroid(test_image);

  BOOST_CHECK(centroid_opt.has_value());
  BOOST_CHECK_CLOSE(centroid_opt.value().x(), expected_centroid_point.x(), 0.001F);
  BOOST_CHECK_CLOSE(centroid_opt.value().y(), expected_centroid_point.y(), 0.001F);
}

BOOST_AUTO_TEST_SUITE_END();
