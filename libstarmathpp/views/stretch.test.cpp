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
#define BOOST_TEST_MODULE "pipeline view stretch unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp> // ranges::to<>
#include <range/v3/view/move.hpp>
#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/views/stretch.hpp>
#include <libstarmathpp/algorithm/stretch/midtone_balance_stretcher.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_view_stretch_tests)

using namespace ranges;
using namespace starmathpp;
using namespace starmathpp::pipeline::views;

/**
 * TODO: Document...
 */
BOOST_AUTO_TEST_CASE(pipeline_subtract_background_image_test) {
  Image test_image(100,100,1,1,50); // 100x100, bg=50

  auto result_images = ranges::views::single(test_image)
      | ranges::views::move
      | stretch(starmathpp::algorithm::MidtoneBalanceStretcher(0.25F))
      | to<std::vector>();


// TODO:...
//  std::vector<Image> expected_result_images = {
//      *generate_test_image(25, 25, 10, 10, 0, 450.5),  // threshold=549.5 -> 1000-549.5 = 450.5
//      *generate_test_image(25, 25, 10, 10, 0, 50.5),   // threshold=49.5 -> 100-49.5 = 50.5
//      *generate_test_image(25, 25, 10, 10, 0, 450.5)   // threshold=549.5 -> 1000-549.5 = 450.5
//  };
//
//  BOOST_TEST(result_images.size() == 3);
//  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
//      expected_result_images.begin(), expected_result_images.end());
}

BOOST_AUTO_TEST_SUITE_END();
