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

#ifndef STARMATHPP_ALGORITHM_INTENSITY_WEIGHTED_CENTROIDER_HPP_
#define STARMATHPP_ALGORITHM_INTENSITY_WEIGHTED_CENTROIDER_HPP_ STARMATHPP_ALGORITHM_INTENSITY_WEIGHTED_CENTROIDER_HPP_

#include <optional>
#include <string>

#include <libstarmathpp/algorithm/centroid/centroider.hpp>
#include <libstarmathpp/image.hpp>
#include <libstarmathpp/point.hpp>

namespace starmathpp::algorithm {

/**
 * Intensity Weighted Centroiding (IWC) is similar to WCoG with a
 * difference that the weighting function, Wij is the intensity
 * distribution of the spot pattern, Iij .
 *
 * In comparison to the CoG method, this algorithm performs a better job
 * under low light level conditions and low background and readout noise.
 *
 * The "Intensity Weighted Centroiding" is described on page 170 of
 * "Topics in Adaptive Optics" - "Advanced Methods for Improving the Efficiency
 * of a Shack Hartmann Wavefront Sensor"
 *
 * See http://cdn.intechopen.com/pdfs-wm/26716.pdf
 */
template<typename ImageType>
class IntensityWeightedCentroider : public Centroider<ImageType> {
 public:
  [[nodiscard]] std::string get_name() const override {
    return "IntensityWeightedCentroider";
  }

  /**
   * TODO: auto as return value?
   */
  [[nodiscard]] std::optional<Point<float>> calculate_centroid(
      const cimg_library::CImg<ImageType> &input_image) const override {

    if (input_image.width() <= 0 || input_image.height() <= 0) {
      throw CentroiderException("No image supplied.");
    }

    Point<float> centroid;

    double I2;
    double I2x = 0;
    double I2y = 0;
    double Ixy2 = 0;

    // NOTE: indices x and y fun from 0..M-1.
    // However, for the calculation 1..M is expected.
    // Therefore, +1 is needed. Otherwise, the first I2x
    // and I2y are not counted.
    cimg_forXY(input_image, x, y)
    {
      I2 = std::pow(input_image(x, y), 2.0);
      I2x += I2 * (x + 1);
      I2y += I2 * (y + 1);
      Ixy2 += I2;
    }

    // NOTE: Since the index of an image runs from 0..M-1,
    //       1 needs to be subtracted again.
    centroid.set_x((float) (I2x / Ixy2) - 1);
    centroid.set_y((float) (I2y / Ixy2) - 1);

    return std::optional<Point<float> > { centroid };
  }
};

}  // namespace starmathpp::algorithm

#endif //STARMATHPP_ALGORITHM_INTENSITY_WEIGHTED_CENTROIDER_HPP_
