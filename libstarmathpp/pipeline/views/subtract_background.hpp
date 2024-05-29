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

#ifndef STARMATHPP_PIPELINE_VIEW_SUBTRACT_BACKGROUND_HPP_
#define STARMATHPP_PIPELINE_VIEW_SUBTRACT_BACKGROUND_HPP_ STARMATHPP_PIPELINE_VIEW_SUBTRACT_BACKGROUND_HPP_

#include <range/v3/view/transform.hpp>

#include <libstarmathpp/pipeline/views/arithmetic_function_template.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/threshold/thresholder.hpp>

#define STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG 0

/**
 *
 */
namespace starmathpp::pipeline::views {

  /**
   * TODO: Change shared_ptrs to unique_ptrs...?!! Because ownership is not shared.
   */
template<typename ImageType = float>
auto subtract_background(const starmathpp::algorithm::Thresholder<ImageType> &thresholder) {
  return ranges::views::transform(
      [&](const std::shared_ptr<const cimg_library::CImg<ImageType>> &image) {
        const auto &input_image_ref = *image;

        DEBUG_IMAGE_DISPLAY(input_image_ref, "subtract_background_in",
                            STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG);

        float threshold = thresholder.calculate_threshold(input_image_ref);

        auto sub_image = std::make_shared < cimg_library::CImg<ImageType>
            > (input_image_ref, "xy");

        Image &sub_image_ref = (*sub_image);

        cimg_forXY(input_image_ref, x, y)
        {
          sub_image_ref(x, y) = (
              input_image_ref(x, y) < threshold ?
                  0 : input_image_ref(x, y) - threshold);
        }

        DEBUG_IMAGE_DISPLAY(sub_image_ref, "subtract_background_out",
                            STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG);

        return sub_image;
      }
  );
}
}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_SUBTRACT_BACKGROUND_HPP_
