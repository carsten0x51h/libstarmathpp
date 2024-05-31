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

#ifndef STARMATHPP_ALGORITHM_STAR_CLUSTER_ALGORITHM_HPP_
#define STARMATHPP_ALGORITHM_STAR_CLUSTER_ALGORITHM_HPP_ STARMATHPP_ALGORITHM_STAR_CLUSTER_ALGORITHM_HPP_

#include <list>
#include <vector>
#include <set>
#include <algorithm>

#include <libstarmathpp/point.hpp>
#include <libstarmathpp/rect.hpp>
#include <libstarmathpp/image.hpp>

namespace starmathpp::algorithm {

// TODO: Should this be here?
typedef Point<int> PixelPos;
typedef std::list<PixelPos> PixelPosList;
typedef std::set<PixelPos> PixelPosSet;

/**
 *
 */
class PixelCluster {
 private:
  PixelPosList pixel_positions_;

 public:
  PixelCluster(const PixelPosList &pixel_positions)
      :
      pixel_positions_(pixel_positions) {
  }
  const PixelPosList& get_pixel_positions() const {
    return pixel_positions_;
  }
  Rect<int> get_bounds() const;
};

/**
 * Usage:
 *
 * CImg<float> binaryImg;
 * StarClusterAlgorithmT starClusterAlgorithm(2);
 * std::list<PixelClusterT> clusters = starClusterAlgorithm.cluster(binaryImg);
 */
class StarClusterAlgorithm {
 private:
  std::vector<PixelPos> offsets_;

  /**
   *
   */
  void init_offset_pattern(int n);

  /**
   * Removes all white neighbours around pixel from whitePixels
   * if they exist and adds them to pixelsToBeProcessed and to
   * pixelsinCluster.
   */
  void
  get_and_remove_neighbours(const PixelPos &cur_pixel_pos,
                            PixelPosSet *white_pixels,
                            PixelPosList *pixels_to_be_processed,
                            PixelPosList *pixel_cluster);

 public:
  explicit StarClusterAlgorithm(size_t cluster_radius);

  std::list<PixelCluster> cluster(const Image &img);
};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_STAR_CLUSTER_ALGORITHM_HPP_
