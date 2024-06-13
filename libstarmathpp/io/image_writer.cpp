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

#include <iostream> // TODO: Remove
#include <memory>

#include <libstarmathpp/io/filesystem_wrapper.hpp>
#include <libstarmathpp/io/image_writer.hpp>
#include <libstarmathpp/io/cimg_fits_io.hpp>

namespace starmathpp::io {

/**
 * Check that the specified path is not an existing directory, that the
 * specified name has an extension, and that an exception is thrown in case
 * the file already exists and override is set to false.
 *
 * @param filepath  File path where the image should be stored to.
 * @param override  Set to true allows overriding an existing file.
 */
void check_filepath(const std::filesystem::path &filepath, bool override) {

  if (is_directory(filepath)) {
    throw ImageWriterException("Specified file path is a directory.");
  } else if (!filepath.has_extension()) {
    throw ImageWriterException("Unable to determine file extension.");
  }
//    	else if (! std::filesystem::exists(filepath.parent_path())) {
//    		// TODO: Maybe sub-folders are automatically created and this check is not required.
//    	    std::stringstream ss;
//    	    ss << "Specified directory '" << filepath.parent_path() << "' does not exist.";
//    		throw ImageWriterException(ss.str());
//    	}
  else if (is_regular_file(filepath) && !override) {
    std::stringstream ss;
    ss << "File '" << filepath << "' already exists and override is disabled.";
    throw ImageWriterException(ss.str());
  }
}

/**
 *
 * @param filepath
 * @param image
 */
void write_fits(const std::string &filepath,
                const cimg_library::CImg<uint16_t> &img, bool override) {

  std::stringstream debugSs;

  try {
    // NOTE: Throws FitsIOException
    starmathpp::io::fits::write(img, filepath, override, &debugSs);
  } catch (starmathpp::io::fits::FitsIOException &exc) {
    std::stringstream ss;
    ss << "Error writing image to '" << filepath
       << "'. FitsIO exception occurred: " << exc.what() << ", ";
    ss << "Details: " << debugSs.str();

    throw ImageWriterException(ss.str());
  }
}

namespace detail {

/**
 *
 */
template<typename ImageType>
void write_internal(const cimg_library::CImg<ImageType> &img, const std::filesystem::path &filepath,
                    bool override) {
  check_filepath(filepath, override);

  const std::string filepath_lower = boost::algorithm::to_lower_copy(
      filepath.string());

  if (starmathpp::io::fits::is_fits(filepath_lower)
      || starmathpp::io::fits::is_fits_gz(filepath_lower)) {
    write_fits(filepath.string(), img, override);
  } else {
    img.save(filepath.string().c_str());
  }

}
}  // namespace detail

/**
 * TODO: Use template specialization here...
 */
void write(const Image &img, const std::filesystem::path &filepath,
           bool override) {
  // NOTE: Storing a float image as TIFF or FITS image results in an
  // image which is probably correct, but which has a range from 0..1
  // and NO program (not even ImageJ) or ImageMagick can display it
  // correctly. Therefore, the float image is converted to a 16 bit
  // image before storing it.
  //
  cimg_library::CImg<uint16_t> uint16_image = img.get_normalize(0, 65535)
      .quantize(65536);

  detail::write_internal(uint16_image, filepath, override);
}

/**
 *
 */
void write(const cimg_library::CImg<uint8_t> &img,
           const std::filesystem::path &filepath, bool override) {
  detail::write_internal(img, filepath, override);
}

/**
 *
 */
void write(const cimg_library::CImg<uint16_t> &img,
           const std::filesystem::path &filepath, bool override) {
  detail::write_internal(img, filepath, override);
}

}
