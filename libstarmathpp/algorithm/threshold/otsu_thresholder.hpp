/*************************************************************
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

#ifndef STARMATHPP_ALGORITHM_OTSU_THRESHOLDER_HPP_
#define STARMATHPP_ALGORITHM_OTSU_THRESHOLDER_HPP_ STARMATHPP_ALGORITHM_OTSU_THRESHOLDER_HPP_

#include <cmath>
#include <string>

#include <libstarmathpp/algorithm/threshold/thresholder.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

namespace starmathpp::algorithm {

/**
 * Otsu’s method assumes that our image contains two classes of pixels: the background and
 * the foreground. Furthermore, Otsu’s method makes the assumption that the grayscale
 * histogram of our pixel intensities of our image is bi-modal, which simply means that
 * the histogram is two peaks.
 *
 * Otsu's thresholding method involves iterating through all the possible threshold
 * values and calculating a measure of spread for the pixel levels each side of the
 * threshold, i.e. the pixels that either fall in foreground or background. The aim
 * is to find the threshold value where the sum of foreground and background spreads
 * is at its minimum.
 *
 * It’s also important to note that Otsu’s method is an example of global thresholding
 * — implying that a single thresholding value is computed for the entire image.
 *
 * This threshold is determined by minimizing intra-class intensity variance, or
 * equivalently, by maximizing inter-class variance. The algorithm exhaustively
 * searches for the threshold that minimizes the intra-class variance, defined
 * as a weighted sum of variances of the two classes
 *
 * See https://rndayala.wordpress.com/2019/11/13/image-processing-thresholding/
 * See http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html
 *
 * IDEA / TODO: int BitDepth as second template parameter?
 * TODO: Use Histogram class?
 */
template<typename ImageType>
class OtsuThresholder : public Thresholder<ImageType> {
 private:
  size_t bit_depth_;

 public:
  /**
   *
   */
  OtsuThresholder(size_t bit_depth)
      :
      bit_depth_(bit_depth) {
  }

  /**
   *
   */
  [[nodiscard]] std::string get_name() const override {
    return "OtsuThresholder";
  }

  /**
   *
   */
  [[nodiscard]] float calculate_threshold(
      const cimg_library::CImg<ImageType> &input_image) const override {

    if (input_image.width() <= 0 || input_image.height() <= 0) {
      throw ThresholderException("No image supplied.");
    }

    auto num_buckets = (size_t) std::pow(2.0F, bit_depth_);
    std::vector < ImageType > hist(num_buckets, 0.0F);

    float num_pixels = input_image.width() * input_image.height();
    float sum_b = 0.0F;
    float wb = 0.0F;
    float max = 0.0F;
    float threshold1 = 0.0F;
    float threshold2 = 0.0F;

    // Calculate histogram - for some reason inImg.get_histogram() behaves unexpectedly.
    cimg_forXY(input_image, x, y)
    {
      int value = (int) input_image(x, y);
      ++hist[value];
    }

    float sum = 0;
    for (size_t pos = 0; pos < num_buckets; ++pos) {
      sum += (float) pos * hist[pos];
    }

    for (size_t i = 0; i < num_buckets; ++i) {
      wb += hist[i];

      if (is_almost_equal(wb, 0.0F)) {
        continue;
      }

      float wf = num_pixels - wb;

      if (is_almost_equal(wf, 0.0F)) {
        break;
      }

      sum_b += (float) i * hist[i];

      float mF = (sum - sum_b) / wf;
      float mB = sum_b / wb;
      float diff = mB - mF;
      float bw = wb * wf * std::pow(diff, 2.0F);

      if (bw >= max) {
        threshold1 = (float) i;
        if (bw > max) {
          threshold2 = (float) i;
        }
        max = bw;
      }
    }  // end loop

    return (threshold1 + threshold2) / 2.0F;
  }
};

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_OTSU_THRESHOLDER_HPP_
