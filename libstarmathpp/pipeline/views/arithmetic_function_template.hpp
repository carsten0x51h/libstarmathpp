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
#include <libstarmathpp/inconsistent_image_dimensions_exception.hpp>

#define STARMATHPP_PIPELINE_ARITHMETIC_FUNCTION_DEBUG 0

namespace starmathpp::pipeline::views {

//  TODO: Idea -> Rename this to ImagePipelineException... - use it in all pipeline functions!
//DEF_Exception(ArithmeticImageOp);

namespace detail {

template<typename ImageType = float>
void throw_if_inconsistent_image_dimensions(
    const std::shared_ptr<cimg_library::CImg<ImageType> > &img_ptr1,
    const std::shared_ptr<cimg_library::CImg<ImageType> > &img_ptr2) {

  if (img_ptr1->width() != img_ptr2->width()
      || img_ptr1->height() != img_ptr2->height()) {
    std::stringstream ss;

    ss << "Inconsistent images dimensions. Initial image dimension: " << "("
        << img_ptr1->width() << ", " << img_ptr1->height() << "), "
        << "new image dimension: (" << img_ptr2->width() << ", "
        << img_ptr2->height() << ").";

    throw InconsistentImageDimensionsException(ss.str());
  }
}

}  // namespace detail

/**
 * Template for image on image operation.
 */
template<template<typename ImageType> class ArithmeticFunctionTraits,
    typename ImageType = float>
auto arithmetic_function_tmpl(const std::shared_ptr<Image> &image_to_add_ptr) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {

        detail::throw_if_inconsistent_image_dimensions<ImageType>(
            image, image_to_add_ptr);

        DEBUG_IMAGE_DISPLAY(*image, "add_image_in",
                            STARMATHPP_PIPELINE_ARITHMETIC_FUNCTION_DEBUG);

        auto result_image = ArithmeticFunctionTraits<ImageType>::calculate(
            image, image_to_add_ptr);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_image_out",
                            STARMATHPP_PIPELINE_ARITHMETIC_FUNCTION_DEBUG);

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
                            STARMATHPP_PIPELINE_ARITHMETIC_FUNCTION_DEBUG);

        auto result_image = ArithmeticFunctionTraits<ImageType>::calculate(
            image, scalar_value_to_add);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_scalar_out",
                            STARMATHPP_PIPELINE_ARITHMETIC_FUNCTION_DEBUG);

        return result_image;
      }
  );
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_ARITHMETIC_FUNCTION_TEMPLATE_H_
