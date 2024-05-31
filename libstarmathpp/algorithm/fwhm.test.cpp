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
#define BOOST_TEST_MODULE "algorithm fwhm unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/fwhm.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_fwhm_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

namespace bdata = boost::unit_test::data;

/**
 * Calculation of the FWHM for different ideal gaussian shaped
 * 2D distribution.
 */
BOOST_DATA_TEST_CASE(algorithm_fwhm_test_ideal_gaussian_sigmaX_test,
    bdata::make(
        std::vector<float> {1.0F, 2.0F, 3.0F, 4.0F, 5.0F}
    )*
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
    sigma, image_dimension, norm_factor_str)
{
  std::stringstream filename_ss;

  filename_ss << "test_data/algorithm/fwhm/gaussian_normal_distribution_2d/gaussian_2d_sigma"
  << (int) sigma << "_" << norm_factor_str << "_odd_"
  << image_dimension << "x" << image_dimension << ".tiff";

  Image input_image(filename_ss.str().c_str());

  double calculated_fwhm = starmathpp::algorithm::fwhm(input_image);
  double expected_fwhm = 2.0 * std::sqrt(std::log(2.0)) * sigma;

  BOOST_CHECK_CLOSE(calculated_fwhm, expected_fwhm, 0.01);
}

BOOST_AUTO_TEST_SUITE_END();
