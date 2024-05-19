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
#define BOOST_TEST_MODULE "pipeline view read unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>

#include <libstarmathpp/pipeline/views/read.hpp>

BOOST_AUTO_TEST_SUITE(pipeline_read_tests)

using namespace starmathpp;
using namespace ranges;



BOOST_AUTO_TEST_CASE(pipeline_read_images_test)
{
    const std::vector<std::string> image_filenames {
        "test_data/image_processing_pipeline/read/test_image_tiff_1_65x85.tiff",
        "test_data/image_processing_pipeline/read/test_image_tiff_2_65x85.tiff",
        "test_data/image_processing_pipeline/read/test_image_fits_1_45x47.fits",
        "test_data/image_processing_pipeline/read/test_image_fits_2_45x47.fits"
    };

    const std::vector< std::pair<int, int> > expected_image_dimensions {
            std::make_pair(65, 85),
            std::make_pair(65, 85),
            std::make_pair(45, 47),
            std::make_pair(45, 47)
    };

    auto img_dimensions = image_filenames
                            | pipeline::views::read()
                            | views::transform(
                                    [](const auto & img_ptr) {
                                        return std::make_pair(img_ptr->width(), img_ptr->height());
                                    })
                            | to<std::vector>();

    BOOST_TEST(img_dimensions == expected_image_dimensions);
}

BOOST_AUTO_TEST_SUITE_END();
