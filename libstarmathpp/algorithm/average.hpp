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

#ifndef STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_
#define STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_ STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_

#include <memory>

#include <libstarmathpp/image.hpp>
#include <range/v3/empty.hpp>

namespace starmathpp::algorithm {

template<class Rng>
std::shared_ptr<Image> average(const Rng &rng) {

  if (ranges::empty(rng)) {
    return nullptr;
  }

  auto sum_image_ptr = std::make_shared < Image > (**ranges::begin(rng), "xy", 0);
  size_t image_count = 0;

  for (const auto &img : rng) {
    (*sum_image_ptr) += *img;
    ++image_count;
  }

  (*sum_image_ptr) /= image_count;

  return sum_image_ptr;
}

}  // namespace starmathpp::algorithm

#endif //FOFI_AVERAGE_H
