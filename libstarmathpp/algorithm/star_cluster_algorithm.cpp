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

#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/minmax.hpp>

#include <libstarmathpp/algorithm/star_cluster_algorithm.hpp>

namespace starmathpp::algorithm {

/**
 *
 */
Rect<int> PixelCluster::get_bounds() const {

  auto x_coordinate_range = pixel_positions_
      | ranges::views::transform([](auto const pixel_pos) {
        return pixel_pos.x();
      });
  auto y_coordinate_range = pixel_positions_
      | ranges::views::transform([](auto const pixel_pos) {
        return pixel_pos.y();
      });

  // Find top-left and bottom right pixel...
  auto [xmin, xmax] = ranges::minmax(x_coordinate_range);
  auto [ymin, ymax] = ranges::minmax(y_coordinate_range);

  int w = xmax - xmin + 1;
  int h = ymax - ymin + 1;

  return Rect<int>(xmin, ymin, w, h);
}

/**
 *
 */
StarClusterAlgorithm::StarClusterAlgorithm(size_t cluster_radius) {
  init_offset_pattern((int) cluster_radius);
}

/**
 *
 */
void StarClusterAlgorithm::init_offset_pattern(int n) {

  offsets_.reserve(n * n - 1);

  for (int i = -n; i <= n; ++i) {
    for (int j = -n; j <= n; ++j) {
      offsets_.emplace_back(i, j);
    }
  }
}

/**
 *
 */
void StarClusterAlgorithm::get_and_remove_neighbours(
    const PixelPos &cur_pixel_pos, PixelPosSet *white_pixels,
    PixelPosList *pixels_to_be_processed, PixelPosList *pixel_cluster) {

  for (const PixelPos &offset : offsets_) {
    PixelPos cur_pix_pos(cur_pixel_pos.x() + offset.x(),
                         cur_pixel_pos.y() + offset.y());

    auto it_pix_pos = white_pixels->find(cur_pix_pos);

    if (it_pix_pos != white_pixels->end()) {
      const PixelPos &cur_white_pix_pos = *it_pix_pos;
      pixels_to_be_processed->push_back(cur_white_pix_pos);
      pixel_cluster->push_back(cur_white_pix_pos);
      white_pixels->erase(it_pix_pos);  // Remove white pixel from "white set" since it has been processed, now
    }
  }
}

/**
 *
 */
std::list<PixelCluster> StarClusterAlgorithm::cluster(const Image &img) {

  std::list<PixelCluster> recognized_clusters;
  PixelPosSet white_pixels;

  cimg_forXY(img, x, y)
  {
    if (img(x, y) != 0) {
      white_pixels.insert(white_pixels.end(), PixelPos(x, y));
    }
  }

  // Iterate over white pixels as long as set is not empty
  while (!white_pixels.empty()) {
    PixelPosList pixel_pos_list;
    PixelPosList pixels_to_be_processed;

    auto it_white_pix_pos = white_pixels.begin();

    pixels_to_be_processed.push_back(*it_white_pix_pos);

    while (!pixels_to_be_processed.empty()) {
      PixelPos cur_pixel_pos = pixels_to_be_processed.front();

      get_and_remove_neighbours(cur_pixel_pos, &white_pixels,
                                &pixels_to_be_processed, &pixel_pos_list);
      pixels_to_be_processed.pop_front();
    }

    // Finally, append the cluster
    if (!pixel_pos_list.empty()) {
      recognized_clusters.push_back(PixelCluster(pixel_pos_list));
    }
  }

  return recognized_clusters;
}

}  // namespace starmathpp::algorithm
