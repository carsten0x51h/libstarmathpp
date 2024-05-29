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
#define BOOST_TEST_MODULE "pipeline view files unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <set>

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/algorithm/set_algorithm.hpp>

#include <libstarmathpp/pipeline/views/files.hpp>

BOOST_AUTO_TEST_SUITE(pipeline_files_tests)

using namespace starmathpp::pipeline;
using namespace starmathpp;
using namespace ranges;

/**
 * NOTE: A Data-testcase was not possible here because having a nested container inside the bdata spec.
 *       didn't compile.
 *
 * See https://stackoverflow.com/questions/63115900/c20-ranges-view-to-vector
 * See https://stackoverflow.com/questions/64938645/transform-temporary-vector-with-range-v3
 */
BOOST_AUTO_TEST_CASE(pipeline_files_filenames_with_no_spaces_test)
{
    const std::vector<std::string> filePaths { "test_data/pipeline/files" };
    const std::set<std::string> expectedFilenames {
                    "test_data/pipeline/files/tiff_file_2.tiff",
                    "test_data/pipeline/files/tiff_file_1.tiff",
    };

    auto results =
          filePaths
            | starmathpp::pipeline::views::files("(.*\\.tiff)")
            | ranges::views::join
            | to<std::set>();

    // NOTE: The order of the contained elements does not matter (the order of elements
    //       returned by files() is undefined).
    std::vector<std::string> diff;
    ranges::set_difference(expectedFilenames, results, diff.begin());

    BOOST_TEST(diff.empty());
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_files_filenames_with_spaces_test)
{
    const std::vector<std::string> filePaths { "test_data/pipeline/files" };
    const std::set<std::string> expectedFilenames { "test_data/pipeline/files/fits file with spaces 1.fits",
                                                    "test_data/pipeline/files/fits file with spaces 2.fits",
                                                    "test_data/pipeline/files/fits_file_1.fits",
                                                    "test_data/pipeline/files/fits_file_2.fits" };

    auto results =
          filePaths
            | starmathpp::pipeline::views::files("(.*\\.fits)")
            | ranges::views::join
            | to<std::set>();

    // NOTE: The order of the contained elements does not matter (the order of elements
    //       returned by files() is undefined).
    std::vector<std::string> diff;
    ranges::set_difference(expectedFilenames, results, diff.begin());

    BOOST_TEST(diff.empty());
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_files_non_exising_extension_test)
{
    const std::vector<std::string> filePaths { "test_data/pipeline/files" };
    const std::set<std::string> expectedFilenames { };

    auto results =
            filePaths
              | starmathpp::pipeline::views::files("(.*\\.xyz)")
              | ranges::views::join
              | to<std::set>();

    // NOTE: The order of the contained elements does not matter (the order of elements
    //       returned by files() is undefined).
    std::vector<std::string> diff;
    ranges::set_difference(expectedFilenames, results, diff.begin());

    BOOST_TEST(diff.empty());
}


/**
 *
 */
BOOST_AUTO_TEST_CASE(pipeline_files_no_extension_filter_test)
{
    const std::vector<std::string> filePaths { "test_data/pipeline/files" };
    const std::set<std::string> expectedFilenames {
            "test_data/pipeline/files/fits file with spaces 1.fits",
            "test_data/pipeline/files/fits file with spaces 2.fits",
            "test_data/pipeline/files/fits_file_1.fits",
            "test_data/pipeline/files/fits_file_2.fits",
            "test_data/pipeline/files/noext_file_1",
            "test_data/pipeline/files/noext_file_2",
            "test_data/pipeline/files/tiff_file_1.tiff",
            "test_data/pipeline/files/tiff_file_2.tiff",
            "test_data/pipeline/files/txt_file_1.txt",
            "test_data/pipeline/files/txt_file_2.txt"
    };

    auto results =
            filePaths
              | starmathpp::pipeline::views::files()
              | ranges::views::join
              | to<std::set>();

    // NOTE: The order of the contained elements does not matter (the order of elements
    //       returned by files() is undefined).
    std::vector<std::string> diff;
    ranges::set_difference(expectedFilenames, results, diff.begin());

    BOOST_TEST(diff.empty());
}

BOOST_AUTO_TEST_SUITE_END();
