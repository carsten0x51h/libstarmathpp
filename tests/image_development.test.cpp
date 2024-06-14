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
#define BOOST_TEST_MODULE "image development integration test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/core.hpp>   // ranges::front()
#include <range/v3/algorithm/for_each.hpp>

#include <range/v3/action/join.hpp>


#include <libstarmathpp/pipeline/views/files.hpp>
#include <libstarmathpp/pipeline/views/read.hpp>
#include <libstarmathpp/pipeline/views/subtract.hpp>
#include <libstarmathpp/pipeline/views/divide_by.hpp>
#include <libstarmathpp/pipeline/views/interpolate_bad_pixels.hpp>
#include <libstarmathpp/pipeline/views/stretch.hpp>

#include <libstarmathpp/pipeline/actions/write.hpp>

#include <libstarmathpp/io/filesystem_wrapper.hpp>

#include <libstarmathpp/algorithm/average.hpp>
#include <libstarmathpp/algorithm/stretch/midtone_balance_stretcher.hpp>

#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (image_development_integration_tests)

using namespace ranges;
using namespace starmathpp;
using namespace starmathpp::io;
using namespace starmathpp::algorithm;
using namespace starmathpp::pipeline::views;

/**
 * IDEA: Develop an astronomical image by averaging the dark frames,
 * averaging the flat-field frames, averaging the dark flat-field frames,
 * subtracting the average dark flat-field frame from each flat-field frame,
 * then average the resulting flat-field frames, subtract the average dark
 * frame from each light frame and divide each light frame by the average
 * flat-field frame. Then average all resulting light frames.
 * In addition, for all frames the bad pixels are interpolated.
 * In a final step the image is auto-stretched using the midtone balance
 * algorithm.
 *
 * NOTE: This does no align the images.
 *
 * TODO / IDEA: Add     | value_clip(ClippingAlgorithmT) after subtract?
 */
BOOST_AUTO_TEST_CASE(pipeline_standard_image_development_test, * boost::unit_test::tolerance(0.1))
{
  const std::string base_path = "test_data/integration/image_development/";

  auto dark_files        = files(base_path + "dark", "(.*\\.fit\\.gz)") | to<std::vector>();
  auto dark_flat_files   = files(base_path + "dark_flat", "(.*\\.fit\\.gz)") | to<std::vector>();
  auto flat_files        = files(base_path + "flat", "(.*\\.fit\\.gz)") | to<std::vector>();
  auto light_frame_files = files(base_path + "light", "(.*\\.fit\\.gz)") | to<std::vector>();

  /**
   * NOTE: If just one pixel has a NAN value (e.g. div by 0), BOOST_TEST() fails when comparing two
   * images, even if both images have a "NAN value" at the same pixel position. The reason is that
   * NAN == NAN is false. Therefore, remove_nans() is the last step which uses a median blur filter
   * to interpolate all NAN values using their surrounding neighbours.
   */
  float BAD_PIXEL_THRESHOLD = 500;
  float FILTER_CORE_SIZE = 3;
  float TARGET_BACKGROUND = 0.06F;

  view::single(
      average(
         light_frame_files
         | read()
         | interpolate_bad_pixels(BAD_PIXEL_THRESHOLD, FILTER_CORE_SIZE)
         | subtract(
             average(dark_files
                 | read()
                 | interpolate_bad_pixels(BAD_PIXEL_THRESHOLD, FILTER_CORE_SIZE)
             )
         )
         | divide_by(
             average(
                 flat_files
                 | read()
                 | interpolate_bad_pixels(BAD_PIXEL_THRESHOLD, FILTER_CORE_SIZE)
                 | subtract(
                     average(dark_flat_files
                         | read()
                         | interpolate_bad_pixels(BAD_PIXEL_THRESHOLD, FILTER_CORE_SIZE)
                     )
                 )
             )
          )
      )
  )
  | stretch(starmathpp::algorithm::MidtoneBalanceStretcher(TARGET_BACKGROUND))
  | starmathpp::pipeline::actions::write<uint8_t>(".", "final_image_%03d.tiff");


  // This line initially generated the expected result.
//  std::string expected_image_filename = base_path + "expected_result.tiff";
//  cimg_library::CImg<uint8_t> expected_result(expected_image_filename.c_str());
//
//  // Just one image is expected as result from the processing pipeline
//  BOOST_TEST(final_image_vec.size() == 1);
//
//  // The ranges::front() call extracts the only image from the range (here a std::shared_ptr<ImageT>).
//  cimg_library::CImg<uint8_t> calculated_img = *final_image_vec.front();
//
//  // TODO / FIXME: When persisting the image and loading the image again
//  //               from disc (the expected result), some rounding causes
//  //               slight differences in the pixel values (+/-1).
//  //               Therefore, the comparison is done by subtracting.
//  //               The conversion to a float image is done because otherwise
//  //               the uint8_t type causes problems in case of negaitve values.
//  Image diff_image = (Image)calculated_img - (Image)expected_result;
//
//  BOOST_TEST(*diff_image.abs() < 2);
}

BOOST_AUTO_TEST_SUITE_END();
