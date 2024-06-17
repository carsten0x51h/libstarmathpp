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

#ifndef STARMATHPP_PIPELINE_VIEW_DIVIDE_BY_H_
#define STARMATHPP_PIPELINE_VIEW_DIVIDE_BY_H_ STARMATHPP_PIPELINE_VIEW_DIVIDE_BY_H_

#include <libstarmathpp/views/arithmetic_function_template.hpp>

namespace starmathpp::pipeline::views {

/**
 *
 */
template<typename ImageType>
struct DivideByTraits {
  static std::string operation_name() {
    return "divide";
  }

  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(
      const std::shared_ptr<cimg_library::CImg<ImageType>> &img_ptr1,
      const std::shared_ptr<cimg_library::CImg<ImageType>> &img_ptr2) {

    return std::make_shared < cimg_library::CImg
        < ImageType >> (img_ptr1->get_div(*img_ptr2));
  }

  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(
      const std::shared_ptr<cimg_library::CImg<ImageType>> &img_ptr1,
      ImageType scalar_value) {
    return std::make_shared < cimg_library::CImg
        < ImageType >> (*img_ptr1 / scalar_value);
  }
};

/**
 *
 */
template<typename ImageType = float>
auto divide_by(const std::shared_ptr<Image> &image_divisor_ptr) {
  return arithmetic_function_tmpl<DivideByTraits, ImageType>(image_divisor_ptr);
}

/**
 *
 */
template<typename ImageType = float>
auto divide_by(ImageType scalar_divisor) {
  return arithmetic_function_tmpl<DivideByTraits, ImageType>(scalar_divisor);
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_DIVIDE_BY_H_
