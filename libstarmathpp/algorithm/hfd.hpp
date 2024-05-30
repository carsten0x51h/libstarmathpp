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

#ifndef STARMATHPP_ALGORITHM_HFD_HPP_
#define STARMATHPP_ALGORITHM_HFD_HPP_ STARMATHPP_ALGORITHM_HFD_HPP_

#include <memory>
#include <string>
#include <utility>
#include <functional>

#include <range/v3/empty.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/point.hpp>
#include <libstarmathpp/rect.hpp>
#include <libstarmathpp/size.hpp>
#include <libstarmathpp/exception.hpp>

namespace starmathpp::algorithm {

DEF_Exception(Hfd);

// TODO: Needed?
// NOTE: mOuterDiameter / (2.0 * sqrt(2))
// TODO: Why 2*sqrt(2)?
static double getMaxHfdLimit(unsigned int inOuterHfdDiameter) {
  return 0.353553390593 * inOuterHfdDiameter;
}

/**
 * HDF calculation
 * https://www.lost-infinity.com/night-sky-image-processing-part-6-measuring-the-half-flux-diameter-hfd-of-a-star-a-simple-c-implementation/
 * https://www.lost-infinity.com/the-half-flux-diameter-hfd-for-a-perfectly-normal-distributed-star/
 * https://www.lost-infinity.com/the-half-flux-diameter-hfd-of-a-plain-image/
 * http://www005.upp.so-net.ne.jp/k_miyash/occ02/halffluxdiameter/halffluxdiameter_en.html
 * http://www.cyanogen.com/help/maximdl/Half-Flux.htm
 *
 * Get all pixels inside a radius
 * http://stackoverflow.com/questions/14487322/get-all-pixel-array-inside-circle
 *
 * TODO: Change further unit tests to DATA-TESTS..., rename / move image files
 * TODO: Rename all .tif to .tiff
 *
 * TODO: Change HFD so that:
 *    -Only quadratic images are accepted
 *    -Always set outerRadius to given image dimensions -> no subimage() call anymore
 *    -Assume that the centroid is in the center of the image
 *
 *      FwhmT fwhm = starImg.fwhm(...)
 *      HfdT hfd = starImg.hfd(...)
 *
 * TODO: The thresholding function should be removed completely. This processing step should be executed
 *       previously, if desired.
 *
 * TODO: Maybe this function should be removed, completely... It is a bit strange to have this kind of
 *       behaviour modelled in here...
 *
 * NOTE: This function assumes that the star center is in the center of the image.
 * NOTE: For even image width / height an inaccuracy is introduced here because there
 *       is no real center pixel.
 *
 * @param inImage
 * @param inOuterDiameter
 * @param inScaleFactor
 * @param outCenteredImg
 * @param inSubBgLevel
 * @return
 */
// TODO: Need one variant where center of star is center of supplied image...
template<typename ImageType>
double hfd(const cimg_library::CImg<ImageType> &input_image,
           const Point<unsigned int> &star_center_px) {

  // TODO: Do not hardcode...
  const unsigned int outer_hfd_diameter_px = 55;  // TODO: Calc?! - depends on pixel size and focal length (and seeing...) WAS 21!!! TODO: At least make this configurable - set from the outside!
  const double scale_factor = 1.0;

  if (input_image.is_empty()) {
    throw HfdException("Empty image supplied.");
  }

  // Crop the part from the supplied image which is needed to calculate the HFD value.
  // A boundary check is performed.
  Rect<int> image_bounds(0, 0, input_image.width(), input_image.height());
  Size<unsigned int> sub_image_size(outer_hfd_diameter_px, outer_hfd_diameter_px);

  // NOTE: Needs to be int because subImageRect x and/or y may be negative
  Rect<int> sub_image_rect = Rect<int>::from_center_point(
      star_center_px.to<int>(), sub_image_size.to<int>());

  bool sub_image_rect_inside_bounds = image_bounds.contains(sub_image_rect);

  if (!sub_image_rect_inside_bounds) {
    std::stringstream ss;
    ss << "Cannot calculate HFD. Rect '" << sub_image_rect
        << "' defined by given star center '" << star_center_px
        << "' is outside image bounds '" << image_bounds << "'." << std::endl;

    throw HfdException(ss.str());
  }

  // Finally, make a copy of the part of the image which is used to calculate the HFD.
  Image sub_img = input_image.get_crop(
      (int) sub_image_rect.x() /*x0*/, (int) sub_image_rect.y() /*y0*/,
      (int) (sub_image_rect.x() + sub_image_rect.width() - 1) /*x1*/,
      (int) (sub_image_rect.y() + sub_image_rect.height() - 1) /*y1*/);

  /**
   * interpolation_type - see https://cimg.eu/reference/structcimg__library_1_1CImg.html
   *
   * -1 = no interpolation: raw memory resizing.
   *  0 = no interpolation: additional space is filled according to boundary_conditions.
   *  1 = nearest-neighbor interpolation.
   *  2 = moving average interpolation.
   *  3 = linear interpolation.
   *  4 = grid interpolation.
   *  5 = cubic interpolation.
   *  6 = lanczos interpolation.
   */
  sub_img.resize((int) (scale_factor * (double) sub_img.width()),
                 (int) (scale_factor * (double) sub_img.height()),
                 -100 /*size_z*/, -100 /*size_c*/, 1 /*interpolation_type*/);

  // Sum up all pixel values in whole circle
  Point<double> scaled_star_center_sub_image_px(
      (double) sub_img.width() / 2.0, (double) sub_img.height() / 2.0);

  double scaled_outer_radius = scale_factor * outer_hfd_diameter_px / 2.0;
  double sum_pixel_values = 0;
  double sum_weighted_dist = 0;

  cimg_forXY(sub_img, x, y)
  {
    double pixel_distance_to_center = std::hypot(
        x + 0.5 - scaled_star_center_sub_image_px.x(),
        y + 0.5 - scaled_star_center_sub_image_px.y());
    bool is_inside_circle = (pixel_distance_to_center <= scaled_outer_radius);

    if (is_inside_circle) {
      sum_pixel_values += sub_img(x, y);
      sum_weighted_dist += sub_img(x, y) * pixel_distance_to_center;
    }
  }

  // NOTE: Multiplying with 2 is required since actually just the HFR is calculated above
  // NOTE: One exception is the case when there is no flux at all (i.e. a totally black image).
  //       In that case the HFD actually does not exist since there would be a division by 0.
  //       Therefore, in that situation NaN is returned.
  return (
      sum_pixel_values > 0.0 ?
          (2.0 * sum_weighted_dist / sum_pixel_values) / scale_factor : NAN);
}

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_HFD_HPP_
