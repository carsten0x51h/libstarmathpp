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

#ifndef STARMATHPP_PIPELINE_VIEW_SUBTRACT_H_
#define STARMATHPP_PIPELINE_VIEW_SUBTRACT_H_ STARMATHPP_PIPELINE_VIEW_SUBTRACT_H_

#include <libstarmathpp/pipeline/views/arithmetic_function_template.hpp>

#define STARMATHPP_PIPELINE_SUBTRACT_DEBUG 0

namespace starmathpp::pipeline::views {

template<typename ImageType>
struct SubtractTraits {
  static std::string operation_name() { return "subtract"; }
  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr1, const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr2) {
     return std::make_shared < cimg_library::CImg<ImageType>> (*img_ptr1 - *img_ptr2);
  }
  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr1, ImageType scalar_value) {
     return std::make_shared < cimg_library::CImg<ImageType>> (*img_ptr1 - scalar_value);
  }
};

template<typename ImageType = float>
auto subtract(const std::shared_ptr<Image> &image_to_subtract_ptr) {
  return arithmetic_function_tmpl<SubtractTraits, ImageType>(image_to_subtract_ptr);
}

template<typename ImageType = float>
auto subtract(ImageType scalar_value_to_subtract) {
  return arithmetic_function_tmpl<SubtractTraits, ImageType>(scalar_value_to_subtract);
}

} // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_SUBTRACT_H_
