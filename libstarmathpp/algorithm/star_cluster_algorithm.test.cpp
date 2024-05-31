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
#define BOOST_TEST_MODULE "algorithm star cluster algorithm unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/star_cluster_algorithm.hpp>

using namespace starmathpp;
using namespace starmathpp::algorithm;

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE (algorithm_star_cluster_algorithm_tests)

/**
 * Two single pixels which have a distance > 2 pixels should be
 * clustered as two separate segments. The test image contains
 * only two white pixels at positions: (x=12, y=23) and (x=23, x=9).
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_two_pixels_test_image)
{
  Image binaryImg("test_data/algorithm/star_cluster/test_image_two_segments_35x35.tiff");

  StarClusterAlgorithm star_cluster_algorithm(
      2 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);
  std::list<PixelCluster> clusters = star_cluster_algorithm.cluster(binaryImg);

  BOOST_TEST(clusters.size() == 2);

  const auto &pixel_cluster1 = clusters.begin();
  const auto &pixel_cluster2 = std::next(clusters.begin());

  BOOST_TEST(pixel_cluster1->get_pixel_positions().size() == 1);
  BOOST_TEST(pixel_cluster2->get_pixel_positions().size() == 1);

  PixelPos c1p1 = *pixel_cluster1->get_pixel_positions().begin();
  BOOST_TEST(c1p1.x() == 12);
  BOOST_TEST(c1p1.y() == 23);

  PixelPos c2p1 = *pixel_cluster2->get_pixel_positions().begin();
  BOOST_TEST(c2p1.x() == 23);
  BOOST_TEST(c2p1.y() == 9);
}

/**
 * A group of connected pixels with less than 2 0 pixels
 * in between should be detected as one segment.
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_single_segment_test_image)
{
  Image binaryImg("test_data/algorithm/star_cluster/test_image_one_segment_35x35.tiff");

  StarClusterAlgorithm star_cluster_algorithm(
      2 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);
  std::list<PixelCluster> clusters = star_cluster_algorithm.cluster(binaryImg);

  // Expect 1 pixel cluster (see image)
  BOOST_TEST(clusters.size() == 1);

  // Check that all expected pixels are in the segment
  std::list<PixelPos> expected_pixels = { PixelPos(9, 16), PixelPos(10, 14),
      PixelPos(10, 15), PixelPos(10, 16), PixelPos(11, 14), PixelPos(11, 15),
      PixelPos(11, 16), PixelPos(11, 17), PixelPos(10, 12), PixelPos(11, 12),
      PixelPos(11, 13), PixelPos(12, 12), PixelPos(12, 13), PixelPos(12, 14),
      PixelPos(12, 15), PixelPos(12, 16), PixelPos(12, 17), PixelPos(13, 12),
      PixelPos(13, 13), PixelPos(13, 14), PixelPos(13, 15), PixelPos(13, 16),
      PixelPos(13, 17), PixelPos(11, 10), PixelPos(11, 11), PixelPos(12, 10),
      PixelPos(12, 11), PixelPos(13, 10), PixelPos(13, 11), PixelPos(14, 10),
      PixelPos(14, 11), PixelPos(14, 12), PixelPos(14, 13), PixelPos(14, 14),
      PixelPos(14, 15), PixelPos(14, 16), PixelPos(14, 17), PixelPos(15, 10),
      PixelPos(15, 11), PixelPos(15, 12), PixelPos(15, 13), PixelPos(15, 14),
      PixelPos(15, 15), PixelPos(15, 16), PixelPos(15, 19), PixelPos(10, 9),
      PixelPos(11, 9), PixelPos(12, 9), PixelPos(13, 9), PixelPos(15, 9),
      PixelPos(16, 9), PixelPos(16, 10), PixelPos(16, 11), PixelPos(16, 12),
      PixelPos(16, 13), PixelPos(16, 14), PixelPos(16, 15), PixelPos(16, 16),
      PixelPos(16, 17), PixelPos(16, 18), PixelPos(17, 11), PixelPos(17, 12),
      PixelPos(17, 13), PixelPos(17, 14), PixelPos(17, 15), PixelPos(17, 16),
      PixelPos(18, 11), PixelPos(18, 12), PixelPos(18, 13), PixelPos(18, 14),
      PixelPos(19, 15) };

  BOOST_TEST(clusters.begin()->get_pixel_positions() == expected_pixels, boost::test_tools::per_element());
}

/**
 * Try to detect three independent segments.
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_three_segments_test)
{
  Image binary_img("test_data/algorithm/star_cluster/test_image_three_segments_35x35.tiff");

  StarClusterAlgorithm star_cluster_algorithm(
      2 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);
  std::list<PixelCluster> clusters = star_cluster_algorithm.cluster(binary_img);

  BOOST_TEST(clusters.size() == 3);

  //
  // Check that all expected pixels are in respective segments.
  //
  // Segment 1
  std::list<PixelPos> expected_pixels_cluster1 = { PixelPos(8, 8), PixelPos(8,
                                                                            9),
      PixelPos(9, 8), PixelPos(9, 9), PixelPos(9, 10), PixelPos(10, 8),
      PixelPos(10, 9), PixelPos(10, 10), PixelPos(11, 7), PixelPos(11, 8),
      PixelPos(12, 6), PixelPos(13, 5) };
  BOOST_TEST(clusters.begin()->get_pixel_positions() == expected_pixels_cluster1, boost::test_tools::per_element());

  // Segment 2
  std::list<PixelPos> expected_pixels_cluster2 = { PixelPos(11, 23), PixelPos(
      11, 25), PixelPos(12, 22), PixelPos(12, 23), PixelPos(12, 24), PixelPos(
      13, 22), PixelPos(13, 23), PixelPos(14, 24) };
  BOOST_TEST(std::next(clusters.begin())->get_pixel_positions() == expected_pixels_cluster2, boost::test_tools::per_element());

  // Segment 3
  std::list<PixelPos> expected_pixels_cluster3 = { PixelPos(22, 9), PixelPos(
      22, 10), PixelPos(23, 9), PixelPos(23, 10), PixelPos(23, 11), PixelPos(24,
                                                                             8),
      PixelPos(24, 10), PixelPos(24, 12), PixelPos(25, 13) };
  BOOST_TEST(std::next(clusters.begin(), 2)->get_pixel_positions() == expected_pixels_cluster3, boost::test_tools::per_element());
}

/**
 * Test different cluster radii.
 */
BOOST_DATA_TEST_CASE(algorithm_star_cluster_algorithm_cluster_radius_test,
    bdata::make(
        // number of dark pixels between two white pixels to form a cluster, expected number of cluster segments
        std::vector< std::tuple<int, int> > {
          { 2, 2},
          { 3, 1}
        }),
    allowed_number_of_dark_pixels_between_white_pixels, expected_number_of_cluster_segments)
{
  Image binary_img("test_data/algorithm/star_cluster/test_image_two_close_segments_35x35.tiff");
  StarClusterAlgorithm star_cluster_algorithm(allowed_number_of_dark_pixels_between_white_pixels);

  BOOST_TEST(star_cluster_algorithm.cluster(binary_img).size() == expected_number_of_cluster_segments);
}

/**
 * Black image - no cluster segments expected.
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_no_cluster_test)
{
  Image binary_img("test_data/algorithm/star_cluster/test_image_all_pixels_0_100x100.tiff");  // black image

  StarClusterAlgorithm star_cluster_algorithm(
      2 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);

  // Expect that no cluster was found
  BOOST_TEST(star_cluster_algorithm.cluster(binary_img).size() == 0);
}

/**
 * Segment in image corner and border. Expect no exception
 * or segfault.
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_corner_test)
{
  Image binaryImg("test_data/algorithm/star_cluster/test_image_six_segments_close_to_borders_35x35.tiff");

  StarClusterAlgorithm star_cluster_algorithm(
      3 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);

  auto clusters = star_cluster_algorithm.cluster(binaryImg);

  BOOST_TEST(clusters.size() == 5);

  //
  // Check that all expected pixels are in respective segments.
  //
  // Segment 1
  std::list<PixelPos> expected_pixels_cluster1 = { PixelPos(0, 0) };

  BOOST_TEST(clusters.begin()->get_pixel_positions() == expected_pixels_cluster1, boost::test_tools::per_element());

  // Segment 2
  std::list<PixelPos> expected_pixels_cluster2 = { PixelPos(0, 30), PixelPos(
      0, 31), PixelPos(0, 32), PixelPos(0, 33), PixelPos(1, 30), PixelPos(1,
                                                                          31),
      PixelPos(1, 32), PixelPos(1, 33), PixelPos(2, 32), PixelPos(2, 33),
      PixelPos(3, 33), PixelPos(0, 34), PixelPos(1, 34), PixelPos(2, 34),
      PixelPos(3, 34), PixelPos(4, 33), PixelPos(4, 34), PixelPos(5, 33),
      PixelPos(5, 34), PixelPos(7, 33), PixelPos(7, 34), PixelPos(8, 33),
      PixelPos(8, 34), PixelPos(9, 33), PixelPos(9, 34) };

  BOOST_TEST(std::next(clusters.begin(), 1)->get_pixel_positions() == expected_pixels_cluster2, boost::test_tools::per_element());

  // Segment 3
  std::list<PixelPos> expected_pixels_cluster3 = { PixelPos(34, 0) };

  BOOST_TEST(std::next(clusters.begin(), 2)->get_pixel_positions() == expected_pixels_cluster3, boost::test_tools::per_element());

  // Segment 4
  std::list<PixelPos> expected_pixels_cluster4 = { PixelPos(34, 15), PixelPos(
      34, 16), PixelPos(34, 17) };

  BOOST_TEST(std::next(clusters.begin(), 3)->get_pixel_positions() == expected_pixels_cluster4, boost::test_tools::per_element());

  // Segment 5
  std::list<PixelPos> expected_pixels_cluster5 = { PixelPos(34, 33), PixelPos(
      34, 34) };

  BOOST_TEST(std::next(clusters.begin(), 4)->get_pixel_positions() == expected_pixels_cluster5, boost::test_tools::per_element());
}

/**
 * Segment in image corner and border. Expect no exception
 * or segfault.
 */
BOOST_AUTO_TEST_CASE(algorithm_star_cluster_algorithm_big_radius_test)
{
  Image binary_img("test_data/algorithm/star_cluster/test_image_six_segments_close_to_borders_35x35.tiff");

  StarClusterAlgorithm star_cluster_algorithm(
      40 /*defines the allowed number of dark pixels between two white pixels until they form a cluster*/);

  // Expect one big segment
  BOOST_TEST(star_cluster_algorithm.cluster(binary_img).size() == 1);
}

BOOST_AUTO_TEST_SUITE_END();
