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

#ifndef STARMATHPP_ALGORITHM_MAX_ENTROPY_THRESHOLDER_HPP_
#define STARMATHPP_ALGORITHM_MAX_ENTROPY_THRESHOLDER_HPP_ STARMATHPP_ALGORITHM_MAX_ENTROPY_THRESHOLDER_HPP_

#include <cmath>
#include <vector>

#include <libstarmathpp/algorithm/threshold/thresholder.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/floating_point_equality.hpp>

namespace starmathpp::algorithm {

/**
 * TODO: Document...
 * TODO: IDEA: Replace parts of the code by the Histogram class.
 */
template<typename ImageType>
class MaxEntropyThresholder : public Thresholder<ImageType> {

 public:
  /**
   *
   */
  [[nodiscard]] std::string get_name() const override {
    return "MaxEntropyThresholder";
  }

  /**
   *
   */
  [[nodiscard]] float calculate_threshold(
      const cimg_library::CImg<ImageType> &input_image) const override {

    if (input_image.width() <= 0 || input_image.height() <= 0) {
      throw ThresholderException("No image supplied.");
    }

    const size_t NUM_BINS = 256;  // TODO: Do not hardcode ...
    size_t num_bins = NUM_BINS;
    std::vector<float> hist(num_bins, 0);

    float max;
    float min = input_image.min_max(max);

    /**
     * IMPORTANT / IDEA: "Shrink" /map the float image pixel values to 256 possible brightness levels (i.e. 256 histogram bins).
     * The reason is that the performance of the "max entropy algorithm" strongly depends on the histogram size / number of bins.
     * Note that the threshold which will be calculated based on this histogram will be the correct one for this "shrinked"
     * histogram. In order to get the threshold for the initial float image, this transformation needs to be reverted
     * later (see comment below).
     */
    cimg_forXY(input_image, x, y)
    {
      int idx = (int) ((float) (num_bins - 1) * (input_image(x, y) - min)
          / (max - min + 1));
      ++hist[idx];
    }

    // Normalize histogram (sum of all is 1)
    float sum = (float) input_image.width() * (float) input_image.height();

    std::vector<float> norm_hist(hist);
    for (auto it = norm_hist.begin(); it != norm_hist.end(); ++it) {
      *it = *it / sum;
    }

    // Calculate accumulated histograms
    std::vector<float> accumulated_hist_black(num_bins, 0);
    std::vector<float> accumulated_hist_white(num_bins, 0);

    float accum_hist_sum = 0.0F;
    for (size_t idx = 0; idx < num_bins; ++idx) {
      accum_hist_sum += norm_hist[idx];

      accumulated_hist_black[idx] = accum_hist_sum;
      accumulated_hist_white[idx] = 1.0F - accum_hist_sum;
    }

    // Find first index of element not 0 in black distribution
    size_t first_bin_idx = 0;

    for (size_t idx = 0; idx < num_bins; ++idx) {
      if (!is_almost_equal(accumulated_hist_black[idx], 0.0F)) {
        first_bin_idx = idx;
        break;
      }
    }

    // Find last index of element not 0 in white distribution
    size_t last_bin_idx = num_bins;

    for (size_t idx = num_bins - 1; idx > first_bin_idx; --idx) {
      if (!is_almost_equal(accumulated_hist_white[idx], 0.0F)) {
        last_bin_idx = idx;
        break;
      }
    }

    float threshold = 0;
    float max_ent = 0;
    float ent_back;
    float ent_obj;
    float tot_ent;

    for (size_t idx = first_bin_idx; idx < last_bin_idx; ++idx) {

      /* Entropy of the background pixels */
      ent_back = 0.0;

      for (size_t ih = 0; ih <= idx; ih++) {
        if (!is_almost_equal(norm_hist[ih], 0.0F)) {
          float c = norm_hist[ih] / accumulated_hist_black[idx];
          ent_back -= c * std::log(c);
        }
      }

      ent_obj = 0.0;

      for (size_t ih = idx + 1; ih < num_bins; ih++) {
        if (!is_almost_equal(norm_hist[ih], 0.0F)) {
          float c = norm_hist[ih] / accumulated_hist_white[idx];
          ent_obj -= c * std::log(c);
        }
      }

      /* Total entropy */
      tot_ent = ent_back + ent_obj;

      if (max_ent < tot_ent) {
        max_ent = tot_ent;
        threshold = (float) idx;
      }
    }

    /**
     * IMPORTANT: The histogram was "shrunk" to 256 values, i.e. float pixel value range was mapped to 256 brightness values.
     * This "shrinking" step needs to be reverted so that the calculated threshold matches the original float image.
     */
    float th2 = min + (threshold / (float) num_bins) * (max - min);

    return th2;
  }

};

}  // namespace starmathpp

#endif // STARMATHPP_ALGORITHM_MAX_ENTROPY_THRESHOLDER_HPP_
