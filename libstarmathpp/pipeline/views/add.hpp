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

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>

#define STARMATHPP_PIPELINE_ADD_DEBUG 0


namespace starmathpp::pipeline::views {

  DEF_Exception(ArithmeticImageOp);

// TODO: Required for all arithmetic operations... -> move to separate file or create a generic template to manage all the arithmetic operations

/**
 *
 * @tparam ImageType
 * @param image_to_add_ptr
 * @return
 *
 * TODO: Check that all images have the same dimension? Could be configured by parameter...
 * TODO / IDEA: -> to basic_arithmetic template?
 */
template<typename ImageType = float>
auto add(const std::shared_ptr<Image> &image_to_add_ptr) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {
        const cimg_library::CImg<ImageType> &input_image_ref = *image;

        if (input_image_ref.width() != image_to_add_ptr->width()
            || input_image_ref.height() != image_to_add_ptr->height()) {
          std::stringstream ss;
          ss << "Cannot add images of different dimensions [("
              << input_image_ref.width() << ", " << input_image_ref.height()
              << ") != (" << image_to_add_ptr->width() << ", "
              << image_to_add_ptr->height() << ")]";

          throw ArithmeticImageOpException(ss.str());
        }

        DEBUG_IMAGE_DISPLAY(input_image_ref, "add_image_in",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        auto result_image = std::make_shared<cimg_library::CImg<ImageType>>(
            input_image_ref + *image_to_add_ptr);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_image_out",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        return result_image;
      }
  );
}

/**
 *
 * @tparam ImageType
 * @param scalarValueToAdd
 * @return
 */
template<typename ImageType = float>
auto add(ImageType scalarValueToAdd) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {
        const cimg_library::CImg<ImageType> &inputImageRef = *image;

        DEBUG_IMAGE_DISPLAY(inputImageRef, "add_scalar_in",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        auto result_image = std::make_shared<cimg_library::CImg<ImageType>>(
            inputImageRef + scalarValueToAdd);

        DEBUG_IMAGE_DISPLAY(*result_image, "add_scalar_out",
                            STARMATHPP_PIPELINE_ADD_DEBUG);

        return result_image;
      }
  );
}
}

#endif // STARMATHPP_PIPELINE_VIEW_ADD_H_
