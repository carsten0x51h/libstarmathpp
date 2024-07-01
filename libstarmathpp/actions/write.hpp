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

#ifndef STARMATHPP_PIPELINE_ACTION_WRITE_H_
#define STARMATHPP_PIPELINE_ACTION_WRITE_H_ STARMATHPP_PIPELINE_ACTION_WRITE_H_

#include <cstdio>
#include <filesystem>

#include <range/v3/all.hpp> // TODO: REMOVE - Needed for rages::forward

#include <range/v3/action/transform.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/io/image_writer.hpp>

#define STARMATHPP_PIPELINE_ACTION_WRITE_DEBUG 0

namespace starmathpp::pipeline::actions {
  /**
   * NOTE: With C++20, the filename can be easily composed like:
   * std::string filename = std::format(imageFilenamePattern, counter++);
   * The template looks slightly different: "img_{:03}.fit"
   * See https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
   *
   * #include <format>
   *
   * TODO: Compose filename from internal, static counter... ? Better idea? How to reset?
   */
  static std::string compose_filename_from_pattern(const std::string &pattern) {
    char buffer[100];

    static unsigned int counter = 0;

    std::snprintf(buffer, sizeof(buffer), pattern.c_str(), counter++);

    return std::string(buffer);
  }

  /**
   *
   */
  template<typename ImageType>
  auto write(const std::filesystem::path &directory,
             const std::string &image_filename_pattern = "img_%03d.fit",
             bool allow_override = true) {

    return ranges::make_action_closure([directory, image_filename_pattern, allow_override](auto&& range) {
        int index = 0;
        for (auto&& image : range) {
//            std::ostringstream oss;
//            oss << directory << '/' << fmt::format(format, index++) << ".jpeg";
//            image->save(oss.str().c_str());
                    DEBUG_IMAGE_DISPLAY(*image, "pipeline_action_write_in",
                                        STARMATHPP_PIPELINE_ACTION_WRITE_DEBUG);

                    std::filesystem::path filepath = directory
                        / compose_filename_from_pattern(image_filename_pattern);

                    starmathpp::io::write(*image, filepath, allow_override);

        }
        return std::forward<decltype(range)>(range); // Ensure we forward the range correctly
    });
//    return ranges::actions::transform(
//        [=](const std::unique_ptr<cimg_library::CImg<ImageType>> &image) {
//
//          DEBUG_IMAGE_DISPLAY(*image, "pipeline_action_write_in",
//                              STARMATHPP_PIPELINE_ACTION_WRITE_DEBUG);
//
//          std::filesystem::path filepath = directory
//              / compose_filename_from_pattern(image_filename_pattern);
//
//          starmathpp::io::write(*image, filepath, allow_override);
//
//          return image;
//        }
//    );
}

}  // namespace starmathpp::pipeline::actions

#endif // STARMATHPP_PIPELINE_ACTION_WRITE_H_
