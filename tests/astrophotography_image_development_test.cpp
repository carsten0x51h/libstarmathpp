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
#define BOOST_TEST_MODULE "astrophotography image development unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/core.hpp>   // ranges::front()
#include <range/v3/algorithm/for_each.hpp>

#include <libstarmathpp/pipeline/views/files.hpp>
#include <libstarmathpp/pipeline/views/read.hpp>
#include <libstarmathpp/pipeline/views/write.hpp>
#include <libstarmathpp/pipeline/views/subtract.hpp>
#include <libstarmathpp/pipeline/views/divide_by.hpp>
#include <libstarmathpp/pipeline/views/interpolate_bad_pixels.hpp>

#include <libstarmathpp/io/filesystem_wrapper.hpp>

// TODO: Use the average() from the pipeline::action?
#include <libstarmathpp/algorithm/average.hpp>

#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_astrophotography_image_development_integration_tests)

using namespace ranges;
using namespace starmathpp;
using namespace starmathpp::io;
using namespace starmathpp::algorithm;
using namespace starmathpp::pipeline::views;

/**
 * IDEA:
 *         AstroImageProcessingPipelineT
 *              .of("light-frames/*.fits")                                                  -> List<img>
 *              .subtract(ImageProcessingPipelineT.of("dark-frames/*.fits").average())      -> List<img>
 *              .divide(ImageProcessingPipelineT.of("flatfield-frames/*.fits").average())   -> List<img>
 *              .average()                                                                  -> img
 *              .stretch(StretcherT::-...)                                                  -> img
 *              .store/save("my-filename.png")
 *
 * NOTE: This does no alignment!
 *
 * TODO: Add     | value_clip(ClippingAlgorithmT) after subtract?
 */
BOOST_AUTO_TEST_CASE(pipeline_standard_image_development_test, * boost::unit_test::tolerance(0.1))
{
  const std::string base_path = "test_data/image_processing_pipeline/real_world/astrophotography/image_development/";

  auto dark_files = view::single(base_path + "dark")
  | files("(.*\\.fit\\.gz)") | view::join | to<std::vector>();

  auto dark_flat_files = view::single(base_path + "dark_flat")
  | files("(.*\\.fit\\.gz)") | view::join | to<std::vector>();

  auto flat_files = view::single(base_path + "flat")
  | files("(.*\\.fit\\.gz)") | view::join | to<std::vector>();

  auto light_frame_files = view::single(base_path + "light")
  | files("(.*\\.fit\\.gz)") | view::join | to<std::vector>();

  /**
   * NOTE: If just one pixel has a NAN value (e.g. div by 0), BOOST_TEST() fails when comparing two
   * images, even if both images have a "NAN value" at the same pixel position. The reason is that
   * NAN == NAN is false. Therefore, remove_nans() is the last step which uses a median blur filter
   * to interpolate all NAN values using their surrounding neighbours.
   */
  auto light_average_no_nans_range =
  view::single(
      average(
          light_frame_files
          | read()
          | interpolate_bad_pixels(500 /*threshold*/, 3 /*filter size*/)
          | subtract(
              average(dark_files
                  | read()
                  | interpolate_bad_pixels(500 /*threshold*/, 3 /*filter size*/)
              )
          )
          | divide_by(
              average(
                  flat_files
                  | read()
                  | interpolate_bad_pixels(500 /*threshold*/, 3 /*filter size*/)
                  | subtract(
                      average(dark_flat_files
                          | read()
                          | interpolate_bad_pixels(500 /*threshold*/, 3 /*filter size*/)
                      )
                  )
              )
          )
      )
  );

  // This line initially generated the expected result.
  std::string expected_image_filename = base_path + "expected_result.tiff";
  Image expected_result(expected_image_filename.c_str());

  // Just one image is expected as result from the processing pipeline
  BOOST_TEST(size(light_average_no_nans_range) == 1);

  // The ranges::front() call extracts the only image from the range (here a std::shared_ptr<ImageT>).
  auto calculated_img = *ranges::front(light_average_no_nans_range);

  BOOST_TEST(is_almost_equal(calculated_img, expected_result, 0.00001));
}

BOOST_AUTO_TEST_SUITE_END();
