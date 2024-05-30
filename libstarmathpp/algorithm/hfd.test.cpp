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
#define BOOST_TEST_MODULE "algorithm hfd unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <memory>
#include <cmath> // sqrt

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/hfd.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_hfd_tests)

using namespace starmathpp;

namespace bdata = boost::unit_test::data;

/**
 * Check HFD of empty image.
 * Expect a HfdExceptionT.
 */
BOOST_AUTO_TEST_CASE(algorithm_hfd_test_empty_image)
{
  Image null_image;
  BOOST_CHECK_THROW(double hfd = starmathpp::algorithm::hfd(null_image, Point<float>(5.0F,5.0F), 51), starmathpp::algorithm::HfdException);
}

/**
 * Check HFD of completely dark image
 *
 * What should be expected as an HFD value?
 *
 * One exception is the case when there is no flux at all (i.e. a totally black image).
 * In that case the HFD actually does not exist since there would be a division by 0.
 * Therefore, NaN is expected.
 *
 * Also see https://www.lost-infinity.com/night-sky-image-processing-part-6-measuring-the-half-flux-diameter-hfd-of-a-star-a-simple-c-implementation/
 */
BOOST_AUTO_TEST_CASE(algorithm_hfd_dark_image_test)
{
  Image dark_image("test_data/algorithm/hfd/test_image_all_values_0_100x100.tiff");

  const float outerDiameter = 21;
  BOOST_CHECK_EQUAL(std::isnan(starmathpp::algorithm::hfd(dark_image, outerDiameter)), true);
}

/**
 * Same as hfd_test_all_pixel_values_equal_1_test, expectation is
 * that pixel value does not make a difference in calculated HFD value.
 *
 * A detailed explanation why the expected HFD equals (2.0F / 3.0F) * outerDiameter
 * can be found here:
 *
 * https://www.lost-infinity.com/the-half-flux-diameter-hfd-of-a-plain-image/
 */
BOOST_DATA_TEST_CASE(algorithm_hfd_test_all_pixel_values_equal_1_test,
    bdata::make(
        std::vector< std::string > {
          "test_data/algorithm/hfd/test_image_all_pixels_1_120x120.tiff",
          "test_data/algorithm/hfd/test_image_all_pixels_65535_120x120.tiff"
        }) *
    bdata::make(
        std::vector< std::tuple<float, float> > {
          { 1.0F, 0.1F},
          { 10.0F, 0.01F},
          { 100.0F, 0.0001F}
        }),
    image_filename, scale_factor, accepted_error)
{
  Image image(image_filename.c_str());

  const float outer_diameter = 99;
  const float expected_hfd = (2.0F / 3.0F) * outer_diameter;

  BOOST_CHECK_CLOSE(starmathpp::algorithm::hfd(image,
          outer_diameter,
          scale_factor),
      expected_hfd,
      accepted_error);
}

/**
 * A detailed explanation of the formula can be found here:
 *
 * https://www.lost-infinity.com/the-half-flux-diameter-hfd-for-a-perfectly-normal-distributed-star/
 *
 * @param sigma
 * @return The expected HFD
 */
static double calc_expected_hfd(double sigma) {
  // NOTE: Using C++20, those constants can be replaced by <numbers> include.
  //       See: https://en.cppreference.com/w/cpp/numeric/constants
  const double FACTOR = 2.0 * std::tgamma(1.5) * std::sqrt(2.0);
  return FACTOR * sigma;
}

/**
 * Calculation of the HFD for an ideal gaussian shaped distribution.
 */
BOOST_DATA_TEST_CASE(algorithm_hfd_test_ideal_gaussian_sigmaX_test,
    bdata::make(
        std::vector< std::tuple<float, float> > {
          { 1.0F, 4.0F},
          { 2.0F, 2.5F},
          { 3.0F, 1.5F},
          { 4.0F, 0.5F},
          { 5.0F, 0.5F}
        }) *
    bdata::make(
        std::vector< int > {
          51, 101
        }) *
    bdata::make(
        std::vector< std::string > {
          "normalized",
          "factor_32767",
          "factor_65535"
        }),
    sigma, max_error, image_dimension, norm_factor_str)
{
  std::stringstream filename_ss;
  filename_ss << "test_data/algorithm/hfd/gaussian_normal_distribution_2d/gaussian_2d_sigma"
  << (int) sigma << "_" << norm_factor_str << "_odd_"
  << image_dimension << "x" << image_dimension << ".tiff";

  Image input_image(filename_ss.str().c_str());

  BOOST_CHECK_CLOSE(
      starmathpp::algorithm::hfd(input_image,
          (double) image_dimension /*outer_diameter*/,
          1.0F /* scale factor */),
      calc_expected_hfd(sigma), max_error);
}

/**
 * Specify invalid "star center" and/or outer diameters.
 */
BOOST_DATA_TEST_CASE(algorithm_hfd_out_of_bounds_test,
    (bdata::make(
            std::vector< Point<float> > {
              Point<float>(120,120),  // star center position completely out of range of the image dimensions, lower right corner
              Point<float>(130,50),// x coordinate out of bounds
              Point<float>(0,0),// star center position inside image dimensions but "outer diameter" outside image boundaries, top left corner
              Point<float>(119,119),// lower right corner
              Point<float>(60,60)
            }) ^
        bdata::make(
            std::vector< int > {
              3, 3, 3, 3,
              151  // star center is valid but outer radius completely exceeds image bounds
            })) *
    bdata::make(
        std::vector< float > {
          1.0F, 10.0F
        }),
    star_center, outer_diameter, scale_factor)
{
  Image image_120x120("test_data/algorithm/hfd/test_image_all_pixels_65535_120x120.tiff");  // 120x120

  BOOST_CHECK_THROW(starmathpp::algorithm::hfd(
          image_120x120 /* pixel index 0..119 */,
          star_center,
          outer_diameter,
          scale_factor),
      starmathpp::algorithm::HfdException);
}

/**
 * Specify valid "star center" and/or outer diameters (corner case).
 */
BOOST_DATA_TEST_CASE(algorithm_hfd_out_star_center_and_outer_diameter_corner_cases_test,
    bdata::make(
        std::vector< Point<float> > {
          Point<float>(1.5F,1.5F),  // Star center position and outer diameter at the border but still valid - top left corner
          Point<float>(1.5F,50),// left border
          Point<float>(118.5F,50),// right border
          Point<float>(118.5F,118.5F)// bottom right corner
        }) *
    bdata::make(
        std::vector< float > {
          1.0F, 10.0F
        }),
    star_center, scale_factor)
{
  Image image_120x120("test_data/algorithm/hfd/test_image_all_pixels_65535_120x120.tiff");  // 120x120

  starmathpp::algorithm::hfd(
      image_120x120 /* pixel index 0..119 */,
      star_center,
      3,
      scale_factor);

  BOOST_CHECK_NO_THROW(starmathpp::algorithm::hfd(
          image_120x120 /* pixel index 0..119 */,
          star_center,
          3,
          scale_factor)
  );
}

BOOST_AUTO_TEST_SUITE_END();
