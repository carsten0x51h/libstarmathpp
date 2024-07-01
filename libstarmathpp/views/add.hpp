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

#ifndef STARMATHPP_PIPELINE_VIEW_ADD_H_
#define STARMATHPP_PIPELINE_VIEW_ADD_H_ STARMATHPP_PIPELINE_VIEW_ADD_H_

#include <libstarmathpp/views/arithmetic_function_template.hpp>

namespace starmathpp::pipeline::views {

/**
 * TODO: Move to details namespace...
 */
template<typename ImageType>
struct AddTraits {
  static std::string operation_name() {
    return "add";
  }

  static auto calculate(
      const cimg_library::CImg<ImageType>&& img1,
      const cimg_library::CImg<ImageType>& img2) {
    return img1 + img2;
  }

  static auto calculate(
      const cimg_library::CImg<ImageType>&& img1, ImageType scalar_value) {
    return img1 + scalar_value;
  }
};

/**
 *
 */
template<typename ImageType = float>
auto add(const Image& image_to_add) {
  return arithmetic_function_tmpl<AddTraits, ImageType>(image_to_add);
}

/**
 *
 */
template<typename ImageType = float>
auto add(ImageType scalar_value_to_add) {
  return arithmetic_function_tmpl<AddTraits, ImageType>(scalar_value_to_add);
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_ADD_H_
