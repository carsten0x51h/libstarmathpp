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

#ifndef STARMATHPP_ALGORITHM_STRETCHER_HPP_
#define STARMATHPP_ALGORITHM_STRETCHER_HPP_ STARMATHPP_ALGORITHM_STRETCHER_HPP_

#include <string>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>

namespace starmathpp::algorithm {

DEF_Exception(Stretcher);

class Stretcher {
 public:
  virtual ~Stretcher() = default;
  [[nodiscard]] virtual std::string get_name() const = 0;

  [[nodiscard]]
  virtual cimg_library::CImg<uint8_t> stretch(
      const cimg_library::CImg<float> &input_image) const = 0;

//  [[nodiscard]]
//  virtual cimg_library::CImg<uint8_t> map(
//      const cimg_library::CImg<uint16_t> &input_image) const = 0;
//
//  [[nodiscard]]
//  virtual cimg_library::CImg<uint8_t> map(
//      const cimg_library::CImg<uint32_t> &input_image) const = 0;
};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_STRETCHER_HPP_
