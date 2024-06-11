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

#ifndef STARMATHPP_PIPELINE_VIEW_FILES_HPP_
#define STARMATHPP_PIPELINE_VIEW_FILES_HPP_ STARMATHPP_PIPELINE_VIEW_FILES_HPP_

#include <regex>

#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/iterator_range.hpp>

#include <libstarmathpp/pipeline/views/arithmetic_function_template.hpp>
#include <libstarmathpp/io/filesystem_wrapper.hpp>

namespace starmathpp::pipeline::views {

namespace fs = std::filesystem;

namespace detail {

/**
 *
 */
auto files_internal(const std::string &root_path,
                    const std::string &extension_regex = "") {

  const std::regex regex(extension_regex);
  const fs::path root(root_path);

  // See https://en.cppreference.com/w/cpp/filesystem/directory_iterator
  // ... These specializations for directory_iterator make it a borrowed_range and a view.
  //
  // NOTE: https://github.com/ericniebler/range-v3/issues/1400
  // See https://godbolt.org/z/-Vu-Md.
  // std::filesystem::iterator is not a std::safe_range, so you must
  // first make it an object to prevent dangling.
  // However, ranges::make_iterator_range() works for now, but is it a good idea?

  return ranges::make_iterator_range(begin(fs::directory_iterator(root)),
                                     end(fs::directory_iterator(root)))
      | ranges::views::filter([](const auto &entry) {
        return fs::is_regular_file(entry);
      })
      | ranges::views::filter(
          [=](const auto &entry) {
            return (extension_regex.empty()
                || std::regex_match(entry.path().string(), regex));
          }) | ranges::views::transform([](const auto &entry) {
        return entry.path().string();
      });
}

}

/**
 * Return the filenames of all files that have the specified extension
 * in the specified directory and all sub-directories.
 *
 * See https://stackoverflow.com/questions/11140483/how-to-get-list-of-files-with-a-specific-extension-in-a-given-folder
 */
auto files(const std::string &extension_regex = "") {
  return ranges::views::transform([=](const std::string &root_path) {
    return detail::files_internal(root_path, extension_regex);
  });
}

/**
 * Return the filenames of all files that have the specified extension
 * in the rootPath directory and all sub-directories.
 *
 * See https://stackoverflow.com/questions/11140483/how-to-get-list-of-files-with-a-specific-extension-in-a-given-folder
 */
auto files(const std::string &root_path,
           const std::string &extension_regex) {
  return detail::files_internal(root_path, extension_regex);
}

}  // namespace starmathpp::pipeline::views

#endif // STARMATHPP_PIPELINE_VIEW_FILES_HPP_
