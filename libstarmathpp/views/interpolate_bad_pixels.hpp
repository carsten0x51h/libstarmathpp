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

#ifndef STARMATHPP_INTERPOLATE_BAD_PIXELS_HPP_
#define STARMATHPP_INTERPOLATE_BAD_PIXELS_HPP_ STARMATHPP_INTERPOLATE_BAD_PIXELS_HPP_

#include <range/v3/view/transform.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/algorithm/bad_pixel_median_interpolator.hpp>

#define STARMATHPP_INTERPOLATE_BAD_PIXELS_DEBUG 0

namespace starmathpp::pipeline::views {

/**
 * If the potential "bad" pixel value deviates more than this "factor" from the mean
 * of the surrounding pixels, then the pixel is removed, i.e. interpolated by the mean
 * value of the surrounding pixels.
 *
 * @param ImageType
 * @param Filter core size (N x N), odd number > 1, allowed values are 3,5,7,9
 * @param absoluteDetectionThreshold
 *
 * @return Image with interpolated bad pixels (unique_ptr<cimg_library::CImg<ImageType>>)
 *
 *
 * TODO: Move the actual algorithm out of this pipeline function to the "algorithms" and also write
 *       unit tests for this filter independent from this pipeline functionality.
 *
 * TODO: Add:
 *      - relativeDetectionThreshold -> Idea supply an object which carries detection threshold type and value -> This way only one function...
 */
template<typename ImageType = float>
auto interpolate_bad_pixels(
    float absolute_detection_threshold = 500,
    unsigned int filter_core_size = 3,
    starmathpp::algorithm::BadPixelMedianInterpolator::ThresholdDirection::TypeE threshold_direction =
        starmathpp::algorithm::BadPixelMedianInterpolator::ThresholdDirection::BOTH) {
  return ranges::views::transform(
      [=](const cimg_library::CImg<ImageType>&& image) {

        DEBUG_IMAGE_DISPLAY(image, "interpolate_bad_pixels_in",
                            STARMATHPP_INTERPOLATE_BAD_PIXELS_DEBUG);

        starmathpp::algorithm::BadPixelMedianInterpolator bad_pixel_median_interpolator(
            absolute_detection_threshold, filter_core_size,
            threshold_direction);

        auto result_image = bad_pixel_median_interpolator.interpolate(std::move(image));

        DEBUG_IMAGE_DISPLAY(*result_image, "interpolate_bad_pixels_out",
                            STARMATHPP_INTERPOLATE_BAD_PIXELS_DEBUG);

        return result_image;
      }
  );
}

}  // namespace starmathpp::pipeline

#endif // STARMATHPP_INTERPOLATE_BAD_PIXELS_HPP_
