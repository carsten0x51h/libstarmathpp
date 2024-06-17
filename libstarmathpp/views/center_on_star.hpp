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

#ifndef STARMATHPP_PIPELINE_VIEW_CENTER_ON_STAR_HPP_
#define STARMATHPP_PIPELINE_VIEW_CENTER_ON_STAR_HPP_ STARMATHPP_PIPELINE_VIEW_CENTER_ON_STAR_HPP_

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/rect.hpp>

#include <libstarmathpp/algorithm/centroid/centroider.hpp>

#include <range/v3/view/transform.hpp>

#define STARMATHPP_PIPELINE_CENTER_ON_STAR_DEBUG 0

namespace starmathpp::pipeline::views {

/**
 * TODO: How to handle case when no centroid could be determined?
 *       -> For the moment an exception is thrown.
 */
template<typename ImageType = float>
auto center_on_star(
    const starmathpp::algorithm::Centroider<ImageType> &centroider) {
  return ranges::views::transform(
      [&](const std::shared_ptr<cimg_library::CImg<ImageType>> &image) {

        const cimg_library::CImg<ImageType> &input_image_ref = *image;

        auto opt_centroid = centroider.calculate_centroid(input_image_ref);

        DEBUG_IMAGE_DISPLAY(input_image_ref, "center_on_star_in",
                            STARMATHPP_PIPELINE_CENTER_ON_STAR_DEBUG);

        if (opt_centroid.has_value()) {
          auto outer_roi = Rect<float>::from_center_point(
              opt_centroid.value(), (float) input_image_ref.width(),
              (float) input_image_ref.height()).template to<int>();

          // get_crop() by default applies a dirichlet boundary_condition (=0). There are other
          // options as well. In this case, the desired behaviour is to assume that all pixel values
          // where the defined sub-frame exceeds the image boundary are assumed to be 0.
          //
          // boundary_conditions	= Can be { 0=dirichlet | 1=neumann | 2=periodic | 3=mirror }
          //
          // - Dirichlet means "0 outside image".
          // - Neumann means "Nearest neighbor outside image" (i.e. null derivative)
          // - Periodic means "Periodic"
          // - Mirror means "Mirrored image outside".
          //
          // See https://github.com/GreycLab/CImg/issues/110
          auto centroid_sub_img =
              std::make_shared<cimg_library::CImg<ImageType>>(
                  input_image_ref.get_crop(
                      outer_roi.x() /*x0*/, outer_roi.y() /*y0*/,
                      outer_roi.x() + outer_roi.width() - 1/*x1*/,
                      outer_roi.y() + outer_roi.height() - 1/*y1*/));

          DEBUG_IMAGE_DISPLAY(*centroid_sub_img, "center_on_star_out",
                              STARMATHPP_PIPELINE_CENTER_ON_STAR_DEBUG);

          return centroid_sub_img;
        } else {
          throw starmathpp::algorithm::CentroiderException(
              "Unable to determine centroid.");
        }
      }
  );
}
}

#endif // STARMATHPP_PIPELINE_VIEW_CENTER_ON_STAR_HPP_
