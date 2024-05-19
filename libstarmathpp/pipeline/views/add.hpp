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

#include <string>

#include <range/v3/view/transform.hpp>

#include <libstarmathpp/pipeline/views/arithmetic_function_template.hpp>

#define STARMATHPP_PIPELINE_ADD_DEBUG 0

namespace starmathpp::pipeline::views {


template<typename ImageType>
struct AddTraits {
  static std::string operation_name() { return "add"; }
  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr1, const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr2) {
     return std::make_shared < cimg_library::CImg<ImageType>> (*img_ptr1 + *img_ptr2);
  }
  static std::shared_ptr<cimg_library::CImg<ImageType>> calculate(const std::shared_ptr<cimg_library::CImg<ImageType>> & img_ptr1, ImageType scalar_value) {
     return std::make_shared < cimg_library::CImg<ImageType>> (*img_ptr1 + scalar_value);
  }
};

template<typename ImageType = float>
auto add(const std::shared_ptr<Image> &image_to_add_ptr) {
  return arithmetic_function_tmpl<AddTraits, ImageType>(image_to_add_ptr);
}

template<typename ImageType = float>
auto add(ImageType scalar_value_to_add) {
  return arithmetic_function_tmpl<AddTraits, ImageType>(scalar_value_to_add);
}

/**
 *
 * @tparam ImageType
 * @param scalarValueToAdd
 * @return
 */
//template<typename ImageType = float>
//auto add(ImageType scalarValueToAdd) {
//  return ranges::views::transform(
//      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {
//        const cimg_library::CImg<ImageType> &inputImageRef = *image;
//
//        DEBUG_IMAGE_DISPLAY(inputImageRef, "add_scalar_in",
//                            STARMATHPP_PIPELINE_ADD_DEBUG);
//
//        auto result_image = std::make_shared < cimg_library::CImg
//            < ImageType >> (inputImageRef + scalarValueToAdd);
//
//        DEBUG_IMAGE_DISPLAY(*result_image, "add_scalar_out",
//                            STARMATHPP_PIPELINE_ADD_DEBUG);
//
//        return result_image;
//      }
//  );
//}
}

#endif // STARMATHPP_PIPELINE_VIEW_ADD_H_
