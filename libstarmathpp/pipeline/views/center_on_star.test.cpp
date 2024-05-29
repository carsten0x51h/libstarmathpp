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
#define BOOST_TEST_MODULE "pipeline view center on star unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>

#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>

#include <libstarmathpp/pipeline/views/center_on_star.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_center_on_star_tests)

using namespace starmathpp;
using namespace starmathpp::pipeline::views;
using namespace ranges;

/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_center_on_star_test)
{
//  Image expected_result_image(10,10,1,1,250);  // 10x10 - bg value 250
//  expected_result_image(4,4) = 65535.0F;// Bright pixel at the center
//  expected_result_image(4,5) = 65535.0F;// Bright pixel at the center
//  expected_result_image(5,4) = 65535.0F;// Bright pixel at the center
//  expected_result_image(5,5) = 65535.0F;// Bright pixel at the center
//
//  auto input_image = generate_test_image(5, 5, 2, 2, 250, 65535.0F);
//
//  auto scaled_images = ranges::view::single(input_image) | scale_up(2.0F)
//      | to<std::vector>();
//
//  // NOTE: Exactly one image is expected
//  BOOST_TEST(scaled_images.size() == 1);
//
//  const Image &result_image = *(scaled_images.at(0));
//
//  BOOST_TEST(result_image.width() == 10);
//  BOOST_TEST(result_image.height() == 10);
//  BOOST_TEST(is_almost_equal(result_image, expected_result_image, 0.00001));
}


BOOST_AUTO_TEST_SUITE_END();
