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

#include <libstarmathpp/algorithm/centroid/intensity_weighted_centroider.hpp>
#include <libstarmathpp/floating_point_equality.hpp>
#include <libstarmathpp/image.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_intensity_weighted_centroider_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

namespace bdata = boost::unit_test::data;

/**
 * Calculate centroids using the intensity weighted center (IWC)
 * method. This array contains pairs of filename and expected centroid.
 * The expected values were cross-checked manually.
 *
 *
 * For a black square image (e.g. test_image_all_pixels_value_1_5x5.tif),
 * the IWC value for a plain image can be easily calculated:
 *
 * Assuming a plain image where all pixel values are A, and the image
 * dimensions are M x M. Then, I2x = sum_ij(I_ij^2*x), I2y = sum_ij(I_ij^2*y)
 * and Ixy2 = sum_ij(I_ij^2) can be calculated as:
 *
 * I2x = I2y = M * A * \frac{M*(M+1)}{2}
 * Ixy2 = M^2 * A
 *
 * With this, the corresponding IWC can be calculated as:
 *
 * xc = yc = \frac{M+1}{2}
 *
 * Since the pixel index starts with 0, 1 needs to be subtracted.
 * That means, as a result, the expected image indices for a plain
 * image of pixel value A and dimension M x M are:
 *
 * xc_idx = yc_idx = \frac{M-1}{2}
 *
 * That means, the pixel A value has no effect n the result.
 * The expected centroid is exactly in the center of the image,
 *
 *
 * <b>Example</b>
 *
 * For a 5x5 image with pixel value 1, the expected calculated indices
 * for the image centroid are:
 *
 * xc_idx = yc_idx = \frac{5-1}{2} = \frac{4}{2} = 2
 *
 * For the 5x5 image (image index starting with 0,0), this is
 * exactly in the center.
 *
 *
 * TODO: Test case where centroider was not able to calculate a centroid,
 *       i.e. returns an "empty" optional...
 * TODO: Rename image files - Encode dimensions - e.g. 120x120 and type
 *       of image (e.g. plain/all_values_1/gaussian). Adapt names in HFD
 *       tests as well. Why? test_image_15.tif does not say anything.
 *
 * TODO: Implement the other centroid algorithms... WCOG, ?? SUB-PIXEL and MOMENT2?!
 */
BOOST_DATA_TEST_CASE(intensity_weighted_centroider_test, bdata::make(
        std::vector< std::tuple<std::string, Point<float> > > {
          { "test_data/algorithm/centroid/test_image_all_pixels_value_1_5x5.tif", Point<float>(2.0F,2.0F)},
          { "test_data/algorithm/centroid/test_image_one_pixel_bottom_right_5x5.tif", Point<float>(3.0F,3.0F)},
          { "test_data/algorithm/centroid/test_image_one_pixel_top_left_10x10.tif", Point<float>(1.0F,1.0F)},
          { "test_data/algorithm/centroid/test_image_two_opposite_pixels_5x5.tif", Point<float>(2.0F,2.0F)},
          { "test_data/algorithm/centroid/test_image_ideal_star_73x65.tif", Point<float>(22.0F,14.0F)},
          { "test_data/algorithm/centroid/test_image_noise_on_the_left_and_one_center_pixel_5x5.tif", Point<float>(1.998034F,2.0F)}
        }),
    image_filename, expected_centroid_point)
{
  IntensityWeightedCentroider<float> centroider;

  Image test_image(image_filename.c_str());
  std::optional<Point<float>> centroid_opt = centroider.calculate_centroid(test_image);

  BOOST_CHECK(centroid_opt.has_value());
  BOOST_CHECK_CLOSE(centroid_opt.value().x(), expected_centroid_point.x(), 0.001F);
  BOOST_CHECK_CLOSE(centroid_opt.value().y(), expected_centroid_point.y(), 0.001F);
}

BOOST_AUTO_TEST_SUITE_END();
