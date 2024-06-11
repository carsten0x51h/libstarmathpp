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
#define BOOST_TEST_MODULE "algorithm midtone balance stretcher unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
//TODO: #include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/stretch/midtone_balance_stretcher.hpp>
//TODO: #include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_midtone_balance_stretcher_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

/**
 *
 */
BOOST_AUTO_TEST_CASE(algorithm_midtone_balance_stretcher_test) {
  Image input_image("test_data/integration/star_recognizer/test_image_star_recognizer_1.fit.gz");

  starmathpp::algorithm::MidtoneBalanceStretcher midtone_balance_stretcher(0.25F);


  cimg_library::CImg<uint8_t> result_image1 = midtone_balance_stretcher.stretch(input_image);
//  cimg_library::CImg<uint8_t> result_image2 = auto_stretch(input_image, 0.1);
//  cimg_library::CImg<uint8_t> result_image3 = auto_stretch(input_image, 0.15);
//  cimg_library::CImg<uint8_t> result_image4 = auto_stretch(input_image, 0.2);
//  cimg_library::CImg<uint8_t> result_image5 = auto_stretch(input_image);
//
//  TODO: BOOST_TEST(is_almost_equal(threshold, 5.0F));
}

// TODO: Test empty image...
// TODO: Add further tests

BOOST_AUTO_TEST_SUITE_END();
