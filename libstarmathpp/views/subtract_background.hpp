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

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/threshold/thresholder.hpp>

#define STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG 0

/**
 *
 */
namespace starmathpp::pipeline::views {

  /**
   *
   */
template<typename ImageType = float>
auto subtract_background(const starmathpp::algorithm::Thresholder<ImageType> &thresholder) {
  return ranges::views::transform(
      [&](const cimg_library::CImg<ImageType>&& input_image) {

        DEBUG_IMAGE_DISPLAY(input_image, "subtract_background_in",
                            STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG);

        float threshold = thresholder.calculate_threshold(std::move(input_image));

        cimg_library::CImg<ImageType> sub_image(input_image, "xy");

        cimg_forXY(input_image, x, y)
        {
          sub_image(x, y) = (
              input_image(x, y) < threshold ?
                  0 : input_image(x, y) - threshold);
        }

        DEBUG_IMAGE_DISPLAY(sub_image, "subtract_background_out",
                            STARMATHPP_PIPELINE_SUBTRACT_BACKGROUND_DEBUG);

        return sub_image;
      }
  );
}
}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_SUBTRACT_BACKGROUND_HPP_
