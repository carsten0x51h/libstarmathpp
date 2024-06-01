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

#ifndef STARMATHPP_PIPELINE_VIEW_DETECT_STARS_HPP_
#define STARMATHPP_PIPELINE_VIEW_DETECT_STARS_HPP_ STARMATHPP_PIPELINE_VIEW_DETECT_STARS_HPP_

#include <libstarmathpp/pipeline/views/arithmetic_function_template.hpp>

#include <memory>
#include <range/v3/view/transform.hpp>

#include <libstarmathpp/image.hpp>

#include <libstarmathpp/algorithm/star_cluster_algorithm.hpp>
#include <libstarmathpp/algorithm/threshold/thresholder.hpp>

#define STARMATHPP_PIPELINE_DETECT_STARS_DEBUG 0

namespace starmathpp::pipeline::views {

/**
 *
 */
template<typename ImageType = float>
auto detect_stars(
    int cluster_radius,
    const starmathpp::algorithm::Thresholder<ImageType> &thresholder,
    unsigned int border) {

  using SharedImage = std::shared_ptr<cimg_library::CImg<ImageType>>;

  return ranges::views::transform(
      [=, &thresholder](const SharedImage &image) {

        const auto &image_ref = *image;

        DEBUG_IMAGE_DISPLAY(image_ref, "detect_stars_in",
                            STARMATHPP_PIPELINE_DETECT_STARS_DEBUG);

        // TODO: Do not hardcode bit depth 16.... make it part of ImageT?
        float threshold = std::ceil(thresholder.calculate_threshold(image_ref));

        // NOTE: CImg threshold function uses >=
        auto binary_img = image_ref.get_threshold(threshold + 1.0F);

        DEBUG_IMAGE_DISPLAY(binary_img, "detect_stars_image",
                            STARMATHPP_PIPELINE_DETECT_STARS_DEBUG);

        starmathpp::algorithm::StarClusterAlgorithm star_cluster_algorithm(
            cluster_radius);
        auto pixel_clusters = star_cluster_algorithm.cluster(binary_img);

        auto rects_vec = pixel_clusters
            | ranges::views::transform([=](const auto &pixel_cluster) {
              return pixel_cluster.get_bounds().expand_to_square().grow(border);
            }) | ranges::to<std::vector>();

        // TODO: Make shred should not be required here...
        return std::make_shared
            < std::pair<const SharedImage, std::vector < Rect<unsigned int>>
                >> (std::make_pair(image, rects_vec));
      }
  );
}
}  // namespace starmathpp::pipeline::views

#endif //STARMATHPP_PIPELINE_VIEW_DETECT_STARS_HPP_
