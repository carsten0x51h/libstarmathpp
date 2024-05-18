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
#define BOOST_TEST_MODULE "image_writer unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/io/filesystem_wrapper.hpp>
#include <libstarmathpp/io/image_writer.hpp>

namespace bdata = boost::unit_test::data;

/**
 *
 */
BOOST_DATA_TEST_CASE(fits_image_writer_test,
    bdata::make(
        std::vector<std::tuple<starmathpp::Image,std::string, std::uintmax_t>> {
          { starmathpp::Image(20 /*width*/, 20 /*height*/, 1, 3, 0), "image_20x20.tiff", 5047},
          { starmathpp::Image(20 /*width*/, 20 /*height*/, 1, 3, 0), "image_20x20.fits", 5760},
          { starmathpp::Image(20 /*width*/, 20 /*height*/, 1, 3, 0), "image_20x20.fits.gz", 432}
        }),
    image, image_filename, expected_file_size)
{
  starmathpp::io::write(image, image_filename, true /*override*/);

  // TODO: Test for overide flag...

  bool file_was_written = std::filesystem::exists(image_filename);
  std::uintmax_t fileSize = fs::file_size(image_filename);

  BOOST_TEST(file_was_written == true);
  BOOST_TEST(fileSize == expected_file_size);
}
