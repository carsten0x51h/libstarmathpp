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
#define BOOST_TEST_MODULE "algorithm math functions unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <libstarmathpp/algorithm/math_functions.hpp>

BOOST_AUTO_TEST_SUITE (algorithm_math_functions_tests)

using namespace starmathpp;
using namespace starmathpp::algorithm;

namespace bdata = boost::unit_test::data;

/**
 *
 */
BOOST_DATA_TEST_CASE(algorithm_math_function_gaussian_test,
    bdata::make(
        std::vector< std::tuple<double, double, double, double, double> > {
          { 1.0, 0.0, 1.0, 0.0, 1.0},
          { 1.0, 1.5, 1.0, 0.0, 1.0},
          { 1.0, 0.0, 2.0, 0.0, 1.0},
          { 1.0, 0.0, 1.0, 1.0, 1.0}
        }) ^
    bdata::make(
        std::vector< double > {
          0.60653065971263342,
          0.60653065971263342 + 1.5,
          0.60653065971263342 * 2.0,
          1.0
        }),
    x, b, p, c, w, expected_result)
{
  BOOST_CHECK_CLOSE(starmathpp::algorithm::gaussian(x, b, p, c, w), expected_result, 0.0001);
}


/**
 *
 */
BOOST_DATA_TEST_CASE(algorithm_math_function_hyperbolic_test,
    bdata::make(
        std::vector< std::tuple<double, double, double, double, double> > {
          { 1.0, 1.0, 1.0, 1.0, 0.0},
          { 1.0, 1.0, 1.0, 1.0, 1.0},
          { 1.0, 1.0, 3.0, 1.0, 1.0}
        }) ^
    bdata::make(
        std::vector< double > {
          1.0,
          2.0,
          4.0
        }),
    x, a, b, c, d, expected_result)
{
  BOOST_CHECK_CLOSE(starmathpp::algorithm::hyperbolic(x, a, b, c, d), expected_result, 0.0001);
}


/**
 *
 */
BOOST_DATA_TEST_CASE(algorithm_math_function_parabolic_test,
    bdata::make(
        std::vector< std::tuple<double, double, double, double> > {
          { 1.0, 1.0, 1.0, 1.0 },
          { -1.0, 1.0, 1.0, 1.0 },
        }) ^
    bdata::make(
        std::vector< double > {
          3.0,
          1.0
        }),
    x, a, b, c, expected_result)
{
  BOOST_CHECK_CLOSE(starmathpp::algorithm::parabolic(x, a, b, c), expected_result, 0.0001);
}

BOOST_AUTO_TEST_SUITE_END();
