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

#ifndef STARMATHPP_ALGORITHM_FWHM_HPP_
#define STARMATHPP_ALGORITHM_FWHM_HPP_ STARMATHPP_ALGORITHM_FWHM_HPP_

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/point.hpp>

namespace starmathpp::algorithm {

DEF_Exception(Fwhm);

namespace detail {
/**
 *
 */
template<typename ImageType>
double fwhm_internal(const cimg_library::CImg<ImageType> &input_image,
                     const Point<float> &star_center, float scale_factor) {

  if (input_image.is_empty()) {
    throw FwhmException("Empty image supplied.");
  }

}
}  // namespace detail

/*+
 *
 */
template<typename ImageType>
double fwhm(const cimg_library::CImg<ImageType> &input_image,
            const Point<float> &star_center, float scale_factor = 1.0F) {
  return detail::fwhm_internal(input_image, star_center, scale_factor);
}

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_FWHM_HPP_
