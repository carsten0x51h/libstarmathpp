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
#define BOOST_TEST_MODULE "pipeline view write unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <range/v3/range/conversion.hpp>

#include <libstarmathpp/views/write.hpp>

#include <libstarmathpp/image.hpp>

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE (pipeline_write_tests)

using namespace starmathpp;
using namespace ranges;

BOOST_AUTO_TEST_CASE(pipeline_write_images_test)
{
  std::vector<starmathpp::Image> images_to_write {
    starmathpp::Image(20 /*width*/, 20 /*height*/, 1, 3, 0),
    starmathpp::Image(400 /*width*/, 400 /*height*/, 1, 3, 0),
    starmathpp::Image(200 /*width*/, 450 /*height*/, 1, 3, 0)
  };

  std::vector<std::tuple<std::string, std::uintmax_t>> expected_results = { {
      "img_000.fit", 5760 }, { "img_001.fit", 325440 },
      { "img_002.fit", 184320 } };

  auto written_images = images_to_write
      | pipeline::views::write<float>(".", "img_%03d.fit",
                                      true /*allowOverride*/)
      | to<std::vector>();

  // TODO: Test allowOverride...

  BOOST_TEST(written_images.size() == images_to_write.size());

  for (const auto & result : expected_results) {
    std::string filename = get<0>(result);
    std::uintmax_t file_size = get<1>(result);

    BOOST_TEST(std::filesystem::exists(filename) == true);
    BOOST_TEST(fs::file_size(filename) == file_size);
  }
}

BOOST_AUTO_TEST_SUITE_END();
