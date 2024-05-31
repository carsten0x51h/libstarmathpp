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
typedef Point<int> PixelPosT;
typedef std::list<PixelPosT> PixelPosListT;
typedef std::set<PixelPosT> PixelPosSetT;

/**
 *
 */
class PixelClusterT {
 private:
  PixelPosListT mPixelPositions;

 public:
  PixelClusterT(const PixelPosListT &pixelPositions)
      :
      mPixelPositions(pixelPositions) {
  }
  const PixelPosListT& getPixelPositions() const {
    return mPixelPositions;
  }
  Rect<int> getBounds() const;
};

/**
 * Usage:
 *
 * CImg<float> binaryImg;
 * StarClusterAlgorithmT starClusterAlgorithm(2);
 * std::list<PixelClusterT> clusters = starClusterAlgorithm.cluster(binaryImg);
 */
class StarClusterAlgorithmT {
 private:
  std::vector<PixelPosT> mOffsets;

  /**
   *
   */
  void initOffsetPattern(int n);

  /**
   * Removes all white neighbours around pixel from whitePixels
   * if they exist and adds them to pixelsToBeProcessed and to
   * pixelsinCluster.
   */
  void
  getAndRemoveNeighbours(const PixelPosT &inCurPixelPos,
                         PixelPosSetT *inoutWhitePixels,
                         PixelPosListT *inoutPixelsToBeProcessed,
                         PixelPosListT *outPixelCluster);

 public:
  explicit StarClusterAlgorithmT(size_t clusterRadius);

  std::list<PixelClusterT> cluster(const Image &inImg);
};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_STAR_CLUSTER_ALGORITHM_HPP_
