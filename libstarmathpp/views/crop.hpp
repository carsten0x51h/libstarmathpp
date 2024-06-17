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

#ifndef STARMATHPP_PIPELINE_VIEW_CROP_HPP_
#define STARMATHPP_PIPELINE_VIEW_CROP_HPP_ STARMATHPP_PIPELINE_VIEW_CROP_HPP_

#include <range/v3/view/transform.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/join.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/point.hpp>
#include <libstarmathpp/rect.hpp>
#include <libstarmathpp/size.hpp>

#define STARMATHPP_PIPELINE_CROP_DEBUG 0


namespace starmathpp::pipeline::views {

/**
 *
 */
namespace detail {
template<typename CropRegionRng, typename ImageType = float>
static auto
//static std::vector<std::shared_ptr<cimg_library::CImg<ImageType>>>
crop_internal(const CropRegionRng crop_regions,
             const std::shared_ptr<cimg_library::CImg<ImageType>> &image) {

  return crop_regions
      | ranges::view::transform(
          [=](const auto &crop_region) {

            // See https://github.com/GreycLab/CImg/issues/110
            return std::make_shared<cimg_library::CImg<ImageType>>(
                image->get_crop(crop_region.x() /*x0*/, crop_region.y() /*y0*/,
                                crop_region.x() + crop_region.width() - 1/*x1*/,
                                crop_region.y() + crop_region.height() - 1/*y1*/
                                ));
          }
      )
	  | ranges::to<std::vector>();
}
}  // namespace detail

template<typename ImageType = float>
auto
//std::vector<std::shared_ptr<cimg_library::CImg<ImageType>>>
crop() {
  return ranges::view::transform(
      [=](const auto &imageRectsPair) {
        auto img = imageRectsPair->first;
        //const std::vector<RectT<int>> & cropRects = imageRectsPair->second;

        // TODO: Fix below...
//	      				return cropInternal(cropRects, img); // std::vector<std::shared_ptr<cimg_library::CImg<ImageType>>>

        return imageRectsPair->second
            | ranges::view::transform(
                [=](const auto &crop_region) {

                  //LOG(debug) << "Cropping region " << crop_region << std::endl;

                  // See https://github.com/GreycLab/CImg/issues/110
                  return std::make_shared<cimg_library::CImg<ImageType>>(
                      img->get_crop(
                          crop_region.x() /*x0*/, crop_region.y() /*y0*/,
                          crop_region.x() + crop_region.width() - 1/*x1*/,
                          crop_region.y() + crop_region.height() - 1/*y1*/
                          ));
                }
            ) | ranges::to<std::vector>();

      });
}

/**
 *
 */
template<typename CropRegionRng, typename ImageType = float>
auto crop(const CropRegionRng &crop_regions) {
  return ranges::view::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType>> &image) {
        return detail::crop_internal(crop_regions, image);
      });
}

/**
 *
 */
template<typename ImageType = float>
auto crop(const Rect<int> &crop_region) {
  return ranges::view::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {
        return detail::crop_internal(ranges::view::single(crop_region), image).at(0);
  });
}

/**
 *
 */
template<typename ImageType = float>
auto crop_from_center(const Size<int> &crop_region) {
  return ranges::view::transform(
      [=](const std::shared_ptr<cimg_library::CImg<ImageType> > &image) {

        const cimg_library::CImg<ImageType> &input_image_ref = *image;

        DEBUG_IMAGE_DISPLAY(input_image_ref, "crop_from_center_in",
                            STARMATHPP_PIPELINE_CROP_DEBUG);

        // TODO: Maybe this calculation can be simplified...
        Point<float> image_center((float) input_image_ref.width() / 2.0F,
                                 (float) input_image_ref.height() / 2.0F);

        Rect<float> rect = Rect<float>::from_center_point(
            image_center, crop_region.to<float>());

        // get_crop() by default applies a dirichlet boundary_condition (=0). There are other
        // options as well. In this case, the desired behaviour is to assume that all pixel values
        // where the defined sub-frame exceeds the image boundary are assumed to be 0.
        //
        // boundary_conditions	= Can be { 0=dirichlet | 1=neumann | 2=periodic | 3=mirror }
        //
        // - Dirichlet means "0 outside image".
        // - Neumann means "Nearest neighbor outside image" (i.e. null derivative)
        // - Periodic means "Periodic"
        // - Mirror means "Mirrored image outside".
        //
        // See https://github.com/GreycLab/CImg/issues/110
        auto cropped_img = std::make_shared<cimg_library::CImg<ImageType>>(
            input_image_ref.get_crop(rect.x() /*x0*/, rect.y() /*y0*/,
                                   rect.x() + rect.width() - 1/*x1*/,
                                   rect.y() + rect.height() - 1/*y1*/));

        DEBUG_IMAGE_DISPLAY(*cropped_img, "crop_from_center_out",
                            STARMATHPP_PIPELINE_CROP_DEBUG);

        return cropped_img;
      }
  );
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_CROP_HPP_
