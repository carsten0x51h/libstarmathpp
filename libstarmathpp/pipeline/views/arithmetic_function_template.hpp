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

#ifndef STARMATHPP_PIPELINE_VIEW_ARITHMETIC_FUNCTION_TEMPLATE_H_
#define STARMATHPP_PIPELINE_VIEW_ARITHMETIC_FUNCTION_TEMPLATE_H_ STARMATHPP_PIPELINE_VIEW_ARITHMETIC_FUNCTION_TEMPLATE_H_

#include <range/v3/view/transform.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>

namespace starmathpp::pipeline::views {

DEF_Exception(ArithmeticImageOp);

/**
 * Template for image on image operation.
 */
template<template<typename ImageType> class ArithmeticFunctionTraits,
    typename ImageType = float>
auto arithmetic_function_tmpl(const std::shared_ptr<Image> &image_to_add_ptr) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {

        if (image->width() != image_to_add_ptr->width()
            || image->height() != image_to_add_ptr->height()) {
          std::stringstream ss;
          ss << "Cannot add images of different dimensions [(" << image->width()
              << ", " << image->height() << ") != ("
              << image_to_add_ptr->width() << ", " << image_to_add_ptr->height()
              << ")]";

          throw ArithmeticImageOpException(ss.str());
        }

        DEBUG_IMAGE_DISPLAY(*image, "add_image_in",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        auto result_image = ArithmeticFunctionTraits<ImageType>::calculate(
            image, image_to_add_ptr);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_image_out",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        return result_image;
      }
  );
}


/**
 * Template for scalar operations on an image.
 */
template<template<typename ImageType> class ArithmeticFunctionTraits,
    typename ImageType = float>
auto arithmetic_function_tmpl(ImageType scalar_value_to_add) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {

        DEBUG_IMAGE_DISPLAY(*image, "add_scalar_in",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        auto result_image = ArithmeticFunctionTraits<ImageType>::calculate(
                    image, scalar_value_to_add);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_scalar_out",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        return result_image;
      }
  );
}


}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_ARITHMETIC_FUNCTION_TEMPLATE_H_
