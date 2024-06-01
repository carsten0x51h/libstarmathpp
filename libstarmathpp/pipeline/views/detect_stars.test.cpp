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
#define BOOST_TEST_MODULE "pipeline view detect stars unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <range/v3/range/conversion.hpp>

#include <boost/test/unit_test.hpp>

#include <libstarmathpp/pipeline/views/detect_stars.hpp>
#include <libstarmathpp/algorithm/threshold/otsu_thresholder.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_views_detect_stars_tests)

using namespace starmathpp;
using namespace ranges;

/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_views_detect_stars_test)
{
  std::vector<ImagePtr> input_images = {
    std::make_shared<Image>("test_data/pipeline/detect_stars/test_two_stars.tiff"),
    std::make_shared<Image>("test_data/pipeline/detect_stars/test_five_stars.tiff")
  };

  auto results = input_images
      | starmathpp::pipeline::views::detect_stars(
          2, /*cluster radius*/
          starmathpp::algorithm::OtsuThresholder<float>(16), 3 /*border*/)
      | to<std::vector>();

  std::vector<Rect<unsigned int>> expected_rects1 = { Rect<unsigned int>(36, 86,
                                                                         11,
                                                                         11),
      Rect<unsigned int>(95, 35, 12, 12) };

  std::vector<Rect<unsigned int>> expected_rects2 = { Rect<unsigned int>(66,
                                                                         269,
                                                                         11,
                                                                         11),
      Rect<unsigned int>(102, 53, 13, 13), Rect<unsigned int>(117, 12, 8, 8),
      Rect<unsigned int>(125, 218, 12, 12), Rect<unsigned int>(300, 219, 9, 9) };

  auto result_rects1 = results[0]->second;
  BOOST_CHECK_EQUAL_COLLECTIONS(result_rects1.begin(), result_rects1.end(),
      expected_rects1.begin(), expected_rects1.end());

  auto result_rects2 = results[1]->second;
  BOOST_CHECK_EQUAL_COLLECTIONS(result_rects2.begin(), result_rects2.end(),
      expected_rects2.begin(), expected_rects2.end());
}

BOOST_AUTO_TEST_SUITE_END();
