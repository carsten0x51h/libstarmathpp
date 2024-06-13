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

#ifndef SOURCE_STARMATHPP_IMAGE_HPP_
#define SOURCE_STARMATHPP_IMAGE_HPP_ SOURCE_STARMATHPP_IMAGE_HPP_

/**
 * We do not want an X11 dependency by default.
 * Use DEFINE_IMAGE_DEBUG to enable CImg debug.
 */
#ifdef DEBUG_IMAGE_DISPLAY_SWITCH
  #define cimg_display 1
  #define DEBUG_IMAGE_DISPLAY(image, title, enable)                 \
    if (enable) {                                                   \
       cimg_library::CImgDisplay debugImageDisp((image).width(),(image).height(),title, 1); \
       debugImageDisp.display(image);                               \
       while (! debugImageDisp.is_closed()) {                       \
         debugImageDisp.wait();                                     \
       }                                                            \
  }

  /**
   * Enable CImg debug output
   */
  #define cimg_verbosity 3
#else
#define cimg_display 0
  #define DEBUG_IMAGE_DISPLAY(imageRef, title, enable)  {}
#endif

/**
 * NOTE: In order to use TIFF, enable the following define. This also requires adding
 *       ${TIFF_LIBRARIES} to the CMakeLists.txt file.
 * See https://stackoverflow.com/questions/3021050/write-tiff-float-images-using-cimg
 *
 * Important:
 * "If you don't define cimg_use_tiff, then loading/saving tiff is not handled natively
 * by CImg, but is done through ImageMagick's convert, and in this case there are
 * limitations to 16bits tiff."
 *
 * See https://github.com/dtschump/CImg/issues/72
 */
#define cimg_use_tiff

#define cimg_use_jpeg
#define cimg_use_png

#include <CImg.h>

// The following undef is needed due to the following compiler problem:
// range/v3/view/transform.hpp:428:23: error: expected ‘>’ before numeric constant
//  428 |         template(bool True = true)(
// The reason is that the CImg library introduces a constant "True".
#undef True

namespace starmathpp {

/**
 * The template mechanism of the CImg class does not help here, since
 * the required bit depth of the image depends on the file to be loaded.
 * This happens at runtime and not at compile time. Therefore, for the
 * moment, the image type is fixed to float to which is considered to be
 * a compromise between accuracy and memory consumption. This way all
 * possible bit depths (2, 8, 16 and 32 bit) are supported.
 *
 * IDEA: Two options to potentially improve this are:
 *
 * Boost any: https://www.boost.org/doc/libs/1_54_0/doc/html/any/s02.html
 * Boost variant: https://www.boost.org/doc/libs/1_54_0/doc/html/variant.html
 */
using Image = cimg_library::CImg<float>;
using ImagePtr = std::shared_ptr<Image>;

}  // end namespace starmathpp

#endif /* SOURCE_STARMATHPP_IMAGE_HPP_ */

