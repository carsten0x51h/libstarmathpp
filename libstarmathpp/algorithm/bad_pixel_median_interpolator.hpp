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

#ifndef STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_
#define STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_ STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_

#include <libstarmathpp/enum_helper.hpp>
#include <libstarmathpp/image.hpp>


namespace starmathpp::algorithm {

/**
 * Usage:
 *
 * BadPixelMedianInterpolator bad_pixel_median_interpolator();
 * auto output_image = bad_pixel_median_interpolator.interpolate(input_image);
 *
 */
class BadPixelMedianInterpolator {
 public:
  /**
   * TODO: Should this be here?
   */
  struct ThresholdDirection {
    enum TypeE {
      POSITIVE,  // Hot pixels
      NEGATIVE,  // Cold pixels
      BOTH,     // Hot- and cold pixels
      _Count
    };

    static const char* asStr(const TypeE &inType) {
      switch (inType) {
        case POSITIVE:
          return "POSITIVE";
        case NEGATIVE:
          return "NEGATIVE";
        case BOTH:
          return "BOTH";
        default:
          return "<?>";
      }
    }

    MAC_AS_TYPE(Type, E, _Count);
  };

  /**
   *
   */
  explicit BadPixelMedianInterpolator(
      float absolute_detection_threshold = 500, unsigned int filter_core_size =
          3,
      ThresholdDirection::TypeE threshold_direction = ThresholdDirection::BOTH)
      :
      absolute_detection_threshold_(absolute_detection_threshold),
      filter_core_size_(filter_core_size),
      threshold_direction_(threshold_direction) {
  }

  /**
   *
   */
  template<typename ImageType>
  std::shared_ptr<cimg_library::CImg<ImageType>> interpolate(
      const std::shared_ptr<cimg_library::CImg<ImageType> > &input_image_ptr) {

    const auto input_image_ref = *input_image_ptr;

    // See https://cimg.eu/reference/loops_Using.html
    cimg_library::CImg<ImageType> neighbourhood(filter_core_size_,
                                                filter_core_size_);

    auto result_image = std::make_shared < cimg_library::CImg
        < ImageType >> (input_image_ref);

    switch (filter_core_size_) {
      case 3: {
        cimg_for3x3(input_image_ref, x, y, 0, 0, neighbourhood, ImageType)
        {
          (*result_image)(x, y) = interpolate_internal(
              input_image_ref(x, y), neighbourhood,
              absolute_detection_threshold_, threshold_direction_);
        }
        break;
      }
      case 5: {
        cimg_for5x5(input_image_ref, x, y, 0, 0, neighbourhood, ImageType)
        {
          (*result_image)(x, y) = interpolate_internal(
              input_image_ref(x, y), neighbourhood,
              absolute_detection_threshold_, threshold_direction_);
        }
        break;
      }
      case 7: {
        cimg_for7x7(input_image_ref, x, y, 0, 0, neighbourhood, ImageType)
        {
          (*result_image)(x, y) = interpolate_internal(
              input_image_ref(x, y), neighbourhood,
              absolute_detection_threshold_, threshold_direction_);
        }
        break;
      }
      case 9: {
        cimg_for9x9(input_image_ref, x, y, 0, 0, neighbourhood, ImageType)
        {
          (*result_image)(x, y) = interpolate_internal(
              input_image_ref(x, y), neighbourhood,
              absolute_detection_threshold_, threshold_direction_);
        }
        break;
      }
        //default:
        // TODO: throw...

    }
    return result_image;
  }

 private:
  float absolute_detection_threshold_;
  unsigned int filter_core_size_;
  ThresholdDirection::TypeE threshold_direction_;

  /**
   * TODO: Move to .cpp file? Or solve it via a #define?
   */
  template<typename ImageType>
  ImageType interpolate_internal(
      ImageType pixel_value, const cimg_library::CImg<ImageType> &neighbourhood,
      float threshold, ThresholdDirection::TypeE thresholdDirection) {
    ImageType med = neighbourhood.median();

    bool want_interpolation = false;

    switch (thresholdDirection) {
      case ThresholdDirection::POSITIVE:
        want_interpolation = ((pixel_value - med) > threshold);
        break;

      case ThresholdDirection::NEGATIVE:
        want_interpolation = ((med - pixel_value) > threshold);
        break;

      case ThresholdDirection::BOTH:
        want_interpolation = (std::abs(pixel_value - med) > threshold);
        break;

        //default:
        // TODO: throw...
    }
    return (want_interpolation ? med : pixel_value);
  }

};

}  // namespace starmathpp

#endif // STARMATHPP_BAD_PIXEL_MEDIAN_INTERPOLATOR_HPP_
