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

#ifndef STARMATHPP_PIPELINE_VIEW_SCALE_HPP_
#define STARMATHPP_PIPELINE_VIEW_SCALE_HPP_ STARMATHPP_PIPELINE_VIEW_SCALE_HPP_

#include <range/v3/view/transform.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/enum_helper.hpp>

#define STARMATHPP_PIPELINE_SCALE_DEBUG 0

namespace starmathpp::pipeline::views {

/**
 * TODO: Where to put the enums? Is in namespace sufficient?
 */
struct ScaleDirection {
  enum TypeE {
    UP,
    DOWN,
    _Count
  };

  static const char* asStr(const TypeE &inType) {
    switch (inType) {
      case UP:
        return "UP";
      case DOWN:
        return "DOWN";
      default:
        return "<?>";
    }
  }

  MAC_AS_TYPE(Type, E, _Count)
  ;
};

/**
 * InterpolationTypeT specifies the method which should be used to
 * interpolate during the scaling process.
 *
 *  https://cimg.eu/reference/structcimg__library_1_1CImg.html
 *       resize 	( 	const int  	size_x,
 *       const int  	size_y = -100,
 *       const int  	size_z = -100,
 *       const int  	size_c = -100,
 *       const int  	interpolation_type = 1
 *         ...
 *
 * interpolation_type	Method of interpolation:
 *
 *  -1 = no interpolation: raw memory resizing.
 *   0 = no interpolation: additional space is filled according to boundary_conditions.
 *   1 = nearest-neighbor interpolation.
 *   2 = moving average interpolation.
 *   3 = linear interpolation.
 *   4 = grid interpolation.
 *   5 = cubic interpolation.
 *   6 = lanczos interpolation.
 */
struct InterpolationType {
  enum TypeE {
    RAW,
    NO_INTERPOLATION,
    NEAREST_NEIGHBOR,
    MOVING_AVERAGE,
    LINEAR,
    GRID,
    CUBIC,
    LANCZOS,
    _Count
  };

  static const char* asStr(const TypeE &inType) {
    switch (inType) {
      case RAW:
        return "RAW";
      case NO_INTERPOLATION:
        return "NO_INTERPOLATION";
      case NEAREST_NEIGHBOR:
        return "NEAREST_NEIGHBOR";
      case MOVING_AVERAGE:
        return "MOVING_AVERAGE";
      case LINEAR:
        return "LINEAR";
      case GRID:
        return "GRID";
      case CUBIC:
        return "CUBIC";
      case LANCZOS:
        return "LANCZOS";
      default:
        return "<?>";
    }
  }

  static const int asInt(const TypeE &inType) {
    switch (inType) {
      case RAW:
        return -1;
      case NO_INTERPOLATION:
        return 0;
      case NEAREST_NEIGHBOR:
        return 1;
      case MOVING_AVERAGE:
        return 2;
      case LINEAR:
        return 3;
      case GRID:
        return 4;
      case CUBIC:
        return 5;
      case LANCZOS:
        return 6;
      default:
        return 1;
    }
  }

  MAC_AS_TYPE(Type, E, _Count)
  ;
};

/**
 *
 * @tparam ImageType
 * @param scaleType
 * @param scaleFactor
 * @return
 */
template<typename ImageType = float>
auto scale(ScaleDirection::TypeE scale_type, float scale_factor,
           InterpolationType::TypeE interpolation_type) {
  return ranges::views::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {

        const cimg_library::CImg<ImageType> &input_image_ref = *image;

        DEBUG_IMAGE_DISPLAY(input_image_ref, "scale_in",
                            STARMATHPP_PIPELINE_SCALE_DEBUG);

        auto scaled_image = std::make_shared < cimg_library::CImg<ImageType>
            > (input_image_ref);

        float factor = (
            scale_type == ScaleDirection::UP ?
                scale_factor : 1.0F / scale_factor);

        scaled_image->resize((int) (factor * (float) input_image_ref.width()),
                             (int) (factor * (float) input_image_ref.height()),
                             -100, -100,
                             InterpolationType::asInt(interpolation_type));

        DEBUG_IMAGE_DISPLAY(input_image_ref, "scale_out",
                            STARMATHPP_PIPELINE_SCALE_DEBUG);

        return scaled_image;
      }
  );
}

/**
 *
 * @tparam ImageType
 * @param scaleFactor
 * @param interpolation_type
 * @return
 */
template<typename ImageType = float>
auto scale_up(float scale_factor, InterpolationType::TypeE interpolation_type =
                  InterpolationType::NEAREST_NEIGHBOR) {
  return scale<ImageType>(ScaleDirection::UP, scale_factor, interpolation_type);
}

/**
 *
 * @tparam ImageType
 * @param scaleFactor
 * @param interpolation_type
 * @return
 */
template<typename ImageType = float>
auto scale_down(float scale_factor,
                InterpolationType::TypeE interpolation_type =
                    InterpolationType::NEAREST_NEIGHBOR) {
  return scale<ImageType>(ScaleDirection::DOWN, scale_factor,
                          interpolation_type);
}
}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_SCALE_HPP_
