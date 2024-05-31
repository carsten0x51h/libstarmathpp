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
#include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/fwhm.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_fwhm_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

/**
 *
 */
BOOST_AUTO_TEST_CASE(algorithm_fwhm_test)
{
  Image image(5, 5, 1, 1, 13);  // 5x5 - All pixels have value 13

  double fwhm = starmathpp::algorithm::fwhm(image);

//  std::vector<ImagePtr> input_images = {
//    std::make_shared<Image>(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
//    std::make_shared<Image>(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
//    std::make_shared<Image>(5, 5, 1, 1, -5)// 5x5 - All pixels have value -5
//  };
//
//  auto average_image_ptr = starmathpp::algorithm::average(input_images);
//  auto expected_image_ptr = std::make_shared < Image
//      > (5, 5, 1, 1, (13 + 10 - 5) / 3);  // 5x5 - All pixels have value 6
//
//  BOOST_TEST(is_almost_equal(*average_image_ptr, *expected_image_ptr, 0.00001));
}

BOOST_AUTO_TEST_SUITE_END();
