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
#define BOOST_TEST_MODULE "pipeline view interpolate bad pixels unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>
//#include <range/v3/view/single.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/pipeline/views/interpolate_bad_pixels.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_interpolate_bad_pixels_tests)

using namespace starmathpp;
using namespace ranges;

/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_interpolate_bad_test)
{
  // TOOD: Implement... define 3 images with hot pixel and cold pixel... -> then ckech results.


//  std::vector<ImagePtr> input_images = {
//    std::make_shared<Image>(5, 5, 1, 1, 13),  // 5x5 - All pixels have value 13
//    std::make_shared<Image>(5, 5, 1, 1, 10),// 5x5 - All pixels have value 10
//    std::make_shared<Image>(5, 5, 1, 1, -10)// 5x5 - All pixels have value -10
//  };
//
//  std::vector<Image> expected_result_images = { Image(5, 5, 1, 1, 6.5F),  // 5x5 - All pixels have value 6.5F
//  Image(5, 5, 1, 1, 5),  // 5x5 - All pixels have value 5
//  Image(5, 5, 1, 1, -5)  // 5x5 - All pixels have value -5
//      };
//
//  auto divisor_image_5x5_value2_ptr1 = std::make_shared < Image
//      > (5, 5, 1, 1, 2);
//
//  // NOTE: | views::indirect causes a segmentation fault
//  auto result_images = input_images
//      | pipeline::views::divide_by(divisor_image_5x5_value2_ptr1)
//      | views::transform([](const auto &img_ptr) {
//        return *img_ptr;
//      }) | to<std::vector>();
//
//  BOOST_TEST(result_images.size() == 3);
//  BOOST_CHECK_EQUAL_COLLECTIONS(result_images.begin(), result_images.end(),
//      expected_result_images.begin(), expected_result_images.end());
}

BOOST_AUTO_TEST_SUITE_END();
