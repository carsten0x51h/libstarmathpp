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

#ifndef STARMATHPP_ALGORITHM_HISTOGRAM_HPP_
#define STARMATHPP_ALGORITHM_HISTOGRAM_HPP_ STARMATHPP_ALGORITHM_HISTOGRAM_HPP_

#include <memory>
#include <vector>
#include <cstdint>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>

namespace starmathpp {

/**
 * TODO: Document ...
 *
 * TODO: operator==, operator !=
 */
template<typename ImageType>
class Histogram {
 private:
  std::vector<uint32_t> histogram_;
  ImageType lower_boundary_;
  ImageType upper_boundary_;

  /**
   *
   */
  void calculate_histogram_internal(
      const cimg_library::CImg<ImageType> &input_image, size_t num_bins) {

    histogram_.resize(num_bins, 0);

    // TODO: Check that num_bins > 0
    // TODO: Check that input_image(x, y) - min_pixel_value does not get smaller than image.min()
    // TODO: Check that max_pixel_value > min_pixel_value
    // TODO: Check that max_pixel_value >= image.max()
    // TODO: Limit image dimensions (i.e. number of pixels)? < e.g. uint32_t (2^32)

    /**
     * IDEA: "Shrink" /map the float image pixel values to 256 possible
     * brightness levels (i.e. 256 histogram bins). The reason is that
     * the performance of the "max entropy algorithm" strongly depends
     * on the histogram size / number of bins. Note that the threshold
     * which will be calculated based on this histogram will be the
     * correct one for this "shrunk" histogram. In order to get the
     * threshold for the initial float image, this transformation needs
     * to be reverted later (see comment below).
     */
    const ImageType delta_max_min = upper_boundary_ - lower_boundary_;

    cimg_forXY(input_image, x, y)
    {
      ImageType pixel_value_minus_min = input_image(x, y) - lower_boundary_;
      float factor = (float) pixel_value_minus_min / (float) delta_max_min;
      size_t idx = (size_t) ((float) (num_bins - 1) * factor);

      ++histogram_[idx];
    }
  }

 public:
  /**
   *
   */
  Histogram(const cimg_library::CImg<ImageType> &input_image, size_t num_bins =
                256) {

    // Gain min- and max values from image and use them as lower- and upper
    // boundary for the histogram.
    lower_boundary_ = input_image.min_max(upper_boundary_);

    calculate_histogram_internal(input_image, num_bins);
  }

  /**
   *
   */
  Histogram(const cimg_library::CImg<ImageType> &input_image,
            ImageType min_pixel_value, ImageType max_pixel_value,
            size_t num_bins = 256)
      :
      lower_boundary_(min_pixel_value),
      upper_boundary_(max_pixel_value) {

    calculate_histogram_internal(input_image, num_bins);
  }

//  Histogram(const cimg_library::CImg<ImageType> & input_image, size_t num_bins = 256, ImageType min_pixel_value, ImageType max_pixel_value) {
//    calculate_histogram_from_image_internal(input_image, num_bins, min_pixel_value, max_pixel_value);
//  }

  [[nodiscard]] size_t get_num_bins() const {
    return histogram_.size();
  }

  [[nodiscard]] ImageType get_lower_boundary() const {
    return lower_boundary_;
  }

  [[nodiscard]] ImageType get_upper_boundary() const {
    return upper_boundary_;
  }

  [[nodiscard]] ImageType get_value(size_t idx) const {
    return histogram_[idx];
  }

};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_HISTOGRAM_HPP_
