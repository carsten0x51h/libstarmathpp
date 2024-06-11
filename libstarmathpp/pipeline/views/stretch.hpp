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

#ifndef STARMATHPP_PIPELINE_VIEW_STRETCH_HPP_
#define STARMATHPP_PIPELINE_VIEW_STRETCH_HPP_ STARMATHPP_PIPELINE_VIEW_STRETCH_HPP_

#include <range/v3/view/transform.hpp> // ranges::views::transform

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/stretch/stretcher.hpp>

#define STARMATHPP_PIPELINE_STRETCHER_DEBUG 0

/**
 *
 */
namespace starmathpp::pipeline::views {

/**
 * TODO: Change shared_ptrs to unique_ptrs...?!! Because ownership is not shared.
 * TODO: Stretcher currently only supports float images...
 */
template<typename ImageType = float>
auto stretch(const starmathpp::algorithm::Stretcher &stretcher) {
  return ranges::views::transform(
      [&](const std::shared_ptr<const cimg_library::CImg<ImageType>> &image) {
        const auto &input_image_ref = *image;

        DEBUG_IMAGE_DISPLAY(input_image_ref, "pipeline_view_stretcher_in",
                            STARMATHPP_PIPELINE_STRETCHER_DEBUG);

        auto stretched_image = std::make_shared < cimg_library::CImg
            < uint8_t >> (stretcher.stretch(input_image_ref));

        DEBUG_IMAGE_DISPLAY(*stretched_image, "pipeline_view_stretcher_out",
                            STARMATHPP_PIPELINE_STRETCHER_DEBUG);

        return stretched_image;
      }
  );
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_STRETCH_HPP_
