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

DEF_Exception(Histogram);

/**
 * TODO: Document ...
 *
 * TODO: operator==, operator !=
 */
template<typename ImageType>
class Histogram {
 private:
  std::vector<uint32_t> histogram_;
  ImageType min_pixel_value_;
  ImageType max_pixel_value_;
  ImageType lower_boundary_;
  ImageType upper_boundary_;

  /**
   *
   */
  void throw_if_num_bins_not_valid(size_t num_bins) const {
    if (num_bins < 1) {
      std::stringstream ss;
      ss << "Number of bins must be at least 1." << std::endl;

      throw HistogramException(ss.str());
    }
  }

  /**
   *
   */
  void throw_if_lower_boundary_is_less_or_equal_upper_boundary_value() const {
    if (lower_boundary_ > upper_boundary_) {
      std::stringstream ss;
      ss << "Lower boundary (" << lower_boundary_
          << ") must be lower or equal than upper boundary (" << upper_boundary_
          << "." << std::endl;

      throw HistogramException(ss.str());
    }
  }

  /**
   *
   */
  void throw_if_lower_boundary_is_greater_than_min_image_pixel() const {
    if (lower_boundary_ > min_pixel_value_) {
      std::stringstream ss;
      ss << "Lower boundary (" << lower_boundary_
          << ") must be lower or equal than minimum pixel (" << min_pixel_value_
          << ") value ." << std::endl;

      throw HistogramException(ss.str());
    }
  }

  /**
   *
   */
  void throw_if_upper_boundary_is_less_than_max_image_pixel() const {
    if (upper_boundary_ < max_pixel_value_) {
      std::stringstream ss;
      ss << "Upper boundary (" << upper_boundary_
          << ") must be higher or equal than maximum pixel (" << max_pixel_value_
          << ") value ." << std::endl;

      throw HistogramException(ss.str());
    }
  }

  /**
   * Calculate histogram idx from pixel value.
   *
   * Example:
   *
   * num_bins = 4
   * lower_boundary = 0, upper_boundary = 99  -> delta_max_min = 100
   *
   * rel_pos = value / delta_max_min
   * idx = num_bins * rel_pos
   *
   * value =  0 -> rel_pos =  0 / 100 = 0.0     -> idx = 4 * 0.00 = 0.00 ->   0
   * value = 10 -> rel_pos = 10 / 100 = 0.1     -> idx = 4 * 0.10 = 0.40 ->   0
   * value = 25 -> rel_pos = 25 / 100 = 0.25    -> idx = 4 * 0.25 = 1.00 ->   1
   * value = 30 -> rel_pos = 30 / 100 = 0.3     -> idx = 4 * 0.30 = 1.20 ->   1
   * value = 60 -> rel_pos = 60 / 100 = 0.6     -> idx = 4 * 0.60 = 2.40 ->   2
   * value = 99 -> rel_pos = 99 / 100 = 0.99    -> idx = 4 * 0.99 = 3.96 ->   3
   */
  size_t calculate_histogram_idx_from_pixel_value_internal(ImageType pixel_value) const {

    throw_if_lower_boundary_is_less_or_equal_upper_boundary_value();
    throw_if_lower_boundary_is_greater_than_min_image_pixel();
    throw_if_upper_boundary_is_less_than_max_image_pixel();

    const ImageType delta_max_min = upper_boundary_ - lower_boundary_ + 1;
    ImageType pixel_value_minus_min = pixel_value - lower_boundary_;
    float factor = (float) pixel_value_minus_min / (float) delta_max_min;
    size_t num_bins = histogram_.size();

    return (size_t) ((float) num_bins * factor);
  }


  ImageType calculate_pixel_value_from_histogram_idx_internal(size_t idx) const {

    //throw_if_lower_boundary_is_less_or_equal_upper_boundary_value();
    //throw_if_lower_boundary_is_greater_than_min_image_pixel();
    //throw_if_upper_boundary_is_less_than_max_image_pixel();

    const ImageType delta_max_min = upper_boundary_ - lower_boundary_ + 1;
    size_t num_bins = histogram_.size();
    float factor = (float) idx / (float) num_bins;
    float pixel_value_minus_min = factor * (float) delta_max_min;

    return pixel_value_minus_min + lower_boundary_;
  }


  /**
   *
   */
  void calculate_histogram_internal(
      const cimg_library::CImg<ImageType> &input_image, size_t num_bins) {

    throw_if_num_bins_not_valid(num_bins);

    histogram_.resize(num_bins, 0);

    // TODO: Check that input_image(x, y) - min_pixel_value does not get smaller than image.min()
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

    cimg_forXY(input_image, x, y)
    {
      size_t idx = calculate_histogram_idx_from_pixel_value_internal(input_image(x, y));

      ++histogram_[idx];
    }
  }



  /**
   *
   */
  double accumulate_idx_internal(size_t from_idx, size_t to_idx) const {

    double sum = 0;

    for (size_t idx = from_idx; idx <= to_idx; idx++) {
      ImageType v = calculate_pixel_value_from_histogram_idx_internal(idx);
      sum += v * histogram_[idx];
    }

    return sum;
  }

  /**
   *
   */
  double accumulate_internal(ImageType from_pixel_value, ImageType to_pixel_value) const {

    size_t from_idx = calculate_histogram_idx_from_pixel_value_internal(from_pixel_value);
    size_t to_idx = calculate_histogram_idx_from_pixel_value_internal(to_pixel_value);

    double sum = 0;

    for (size_t idx = from_idx; idx <= to_idx; idx++) {
      ImageType v = calculate_pixel_value_from_histogram_idx_internal(idx);
      sum += v * histogram_[idx];
    }

    return sum;
  }


 public:
  /**
   *
   */
  Histogram(const cimg_library::CImg<ImageType> &input_image, size_t num_bins =
                256) {

    // Gain min- and max values from image. They are used for checking
    // and in this ctor they are used as lower- and upper
    // boundary for the histogram.
    min_pixel_value_ = input_image.min_max(max_pixel_value_);

    lower_boundary_ = min_pixel_value_;
    upper_boundary_ = max_pixel_value_;

    calculate_histogram_internal(input_image, num_bins);
  }

  /**
   *
   */
  Histogram(const cimg_library::CImg<ImageType> &input_image,
            ImageType lower_boundary, ImageType upper_boundary,
            size_t num_bins = 256)
      :
      lower_boundary_(lower_boundary),
      upper_boundary_(upper_boundary) {

    // Gain min- and max values from image. They are used for checking.
    min_pixel_value_ = input_image.min_max(max_pixel_value_);

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

  [[nodiscard]] double accumulate_idx(size_t from_idx, size_t to_idx) const {
    return accumulate_idx_internal(from_idx, to_idx);
  }
  [[nodiscard]] double accumulate_idx(size_t to_idx) const {
    return accumulate_idx_internal(0, to_idx);
  }

  [[nodiscard]] double accumulate(ImageType from_pixel_value, ImageType to_pixel_value) const {
    return accumulate_internal(from_pixel_value, to_pixel_value);
  }
  [[nodiscard]] double accumulate(ImageType to_pixel_value) const {
    return accumulate_internal(min_pixel_value_, to_pixel_value);
  }
  [[nodiscard]] double accumulate() const {
    return accumulate_internal(min_pixel_value_, max_pixel_value_);
  }

};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_HISTOGRAM_HPP_
