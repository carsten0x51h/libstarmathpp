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

#include <libstarmathpp/algorithm/centroid/intensity_weighted_centroider.hpp>
#include <libstarmathpp/pipeline/views/center_on_star.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_center_on_star_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;
using namespace starmathpp::pipeline::views;
using namespace ranges;

/**
 * This test applies the center_on_star() pipeline functionality on a test image
 * with one single star in the upper left corner. The expected result is an new
 * image where the center of the star is in the center of the image. Furthermore,
 * the expected image size is exactly the same. The "newly" created pixels are
 * expected to have the value 0 (e.g. top, left corner).
 */
BOOST_AUTO_TEST_CASE(pipeline_center_on_star_test)
{
  auto test_image_ptr = std::make_shared<Image>("test_data/image_processing_pipeline/center_on_star/test_image_ideal_star_73x65.tiff");

  auto result_images = ranges::views::single(test_image_ptr)
      | center_on_star(IntensityWeightedCentroider<float>())
      | to<std::vector>();

  BOOST_TEST(result_images.size() == 1);

  const Image &result_image = *(result_images.at(0));

  BOOST_TEST(result_image.width() == 73);
  BOOST_TEST(result_image.height() == 65);
  BOOST_TEST(result_image(36, 32) == 65535);
  BOOST_TEST(result_image(0, 0) == 0);
}

BOOST_AUTO_TEST_SUITE_END();
