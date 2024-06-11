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

#ifndef STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_HPP_
#define STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_HPP_ STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_HPP_

#include <libstarmathpp/algorithm/stretch/stretcher.hpp>
#include <libstarmathpp/image.hpp>

#define STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_DEBUG 1

namespace starmathpp::algorithm {

/**
 * IDEA:
 *
 *
 * auto result_images = files("*.fit.gz")
 *                      | read()
 *                      | stretch/map_luminosity() / map(Mapper & mapper) / stretch(curve)
 *                       -> curve = MidtoneStretcher<unsigned char>(0.25F)
 * -> mapper = MidtoneStretcher<unsigned char>(0.25F)
 *
 * -> map is too generic...
 *
 * -> stretch(curve)  // <- If curve which is to be used does not depend on input image
 * -> stretch(const Stretcher &)// <- Image stretching algorithm which can potentially depend on the input image
 * -> | stretch(starmathpp::algorithm::MidtoneStretcher(0.25F))
 *
 *
 * -> TODO: What will stretch do for an RBG image? -> apply stretch per channel
 * -> TODO: What will Thresholder do for an RGB image? -> Only makes sense for a gray image
 */
class MidtoneBalanceStretcher : public Stretcher {

 private:
  /**
   *
   */
  float target_background_;

  /**
   * Defined for 0 <= x <= 1
   *
   * See https://pixinsight.com/doc/tools/HistogramTransformation/HistogramTransformation.html
   * See https://siril.readthedocs.io/en/latest/processing/stretching.html#midtone-transfer-function-transformation-mtf
   * See https://pixinsight.com/doc/tools/AutoHistogram/AutoHistogram.html
   */
  float midtone_transfer_function(float x, float m, float lo, float hi) const {
    if (x <= lo) {
      return 0.0F;
    }

    if (x >= hi) {
      return 1.0F;
    }

    float xp = (x - lo) / (hi - lo);

    return ((m - 1.0F) * xp) / (((2.0F * m - 1.0F) * xp) - m);
  }

  /**
   *
   */
  float clip_to_zero(float v) const {
    return (v < 0.0F ? 0.0F : v);
  }

  /**
   *
   */
  std::tuple<float, float, float> find_midtones_balance(
      const cimg_library::CImg<float> &input_image, float target_background =
          0.25F) const {

#define shadowsClipping -2.80f /* Shadows clipping point measured in sigma units from the main histogram peak. */
    //#define targetBackground 0.25f /* final "luminance" of the image for autostretch in the [0,1] range */

    using namespace cimg_library;

    // The MAD is defined as the median of the absolute deviations from the
    // data's median: MAD = median (| Xi − median(X) |)
    auto median = input_image.median();
    auto median_diff_image = input_image - median;
    auto mad = median_diff_image.abs().median();

    // this is a guard to avoid breakdown point
    if (mad == 0.0F) {
      mad = 0.001F;
    }

    float shadows = clip_to_zero(median + shadowsClipping * mad);
    float m = midtone_transfer_function(median - shadows, target_background,
                                        0.0F, 1.0F);

    // TODO: Remove
    std::cerr << "m=" << m << ", shadows: " << shadows << ", highlights: "
        << 1.0F << std::endl;

    return {m, shadows, 1.0F};
  }

 public:
  /**
   *
   */
  MidtoneBalanceStretcher(float target_background = 0.25)
      :
      target_background_(target_background) {
  }

  /**
   *
   */
  [[nodiscard]] std::string get_name() const override {
    return "MidtoneBalanceStretcher";
  }

  /**
   * Maps a float or 16 bit image to an 8 bit image which can be displayed 1:1.
   *
   * TODO: Document...
   */
  [[nodiscard]]
  cimg_library::CImg<uint8_t> stretch(
      const cimg_library::CImg<float> &input_image) const override {

    if (input_image.width() <= 0 || input_image.height() <= 0) {
      throw StretcherException("No image supplied.");
    }

    using namespace cimg_library;

    DEBUG_IMAGE_DISPLAY(input_image, "midtone_balance_stretcher_in",
                        STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_DEBUG);

    CImg<float> normalized_image = input_image.get_normalize(0.0F, 1.0F);
    auto [midtone, shadows, highlights] = find_midtones_balance(
        normalized_image, target_background_);

    CImg<uint8_t> dest_image(input_image.width(), input_image.height(), 1, 1,
                             0);

    cimg_forXY(normalized_image, x, y)
    {
      dest_image(x, y) = 255
          * midtone_transfer_function(normalized_image(x, y), midtone, shadows,
                                      highlights);
    }

    DEBUG_IMAGE_DISPLAY(dest_image, "midtone_balance_stretcher_out",
                        STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_DEBUG);

    // TODO: Avoid copy.... maybe move semantics?
    return dest_image;
  }

};

}
// namespace starmathpp

#endif // STARMATHPP_ALGORITHM_MIDTONE_BALANCE_STRETCHER_HPP_
