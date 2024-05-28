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

#ifndef STARMATHPP_ALGORITHM_MEAN_THRESHOLDER_HPP_
#define STARMATHPP_ALGORITHM_MEAN_THRESHOLDER_HPP_ STARMATHPP_ALGORITHM_MEAN_THRESHOLDER_HPP_

#include <libstarmathpp/algorithm/threshold/thresholder.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

namespace starmathpp::algorithm {

template<typename ImageType>
class MeanThresholder : public Thresholder<ImageType> {

 public:
  /**
   *
   */
  [[nodiscard]] std::string get_name() const override {
    return "MeanThresholder";
  }

  /**
   *
   */
  [[nodiscard]] float calculate_threshold(
      const cimg_library::CImg<ImageType> &input_image) const override {

    if (input_image.width() <= 0 || input_image.height() <= 0) {
      throw ThresholderException("No image supplied.");
    }

    return (float) input_image.mean();
  }

};

}  // namespace starmathpp

#endif // STARMATHPP_ALGORITHM_MEAN_THRESHOLDER_HPP_
