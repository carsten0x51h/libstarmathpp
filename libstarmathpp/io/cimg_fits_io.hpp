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

#ifndef STARMATHPP_CIMG_FITS_IO_H_
#define STARMATHPP_CIMG_FITS_IO_H_ STARMATHPP_CIMG_FITS_IO_H_

#include <memory>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/image.hpp>

namespace starmathpp::io::fits {

DEF_Exception(FitsIO);

/**
 * @param filepath_lower
 * @return
 */
bool is_fits(const std::string &filepath_lower);

/**
 *
 * @param filepath_lower
 * @return
 */
bool is_fits_gz(const std::string &filepath_lower);

/**
 * CCfits helper function
 * See http://heasarc.gsfc.nasa.gov/fitsio/ccfits/html/cookbook.html
 *
 * TODO: Add/adapt unit tests? -> Load, save and load -> compare...
 * TODO: Improve error handling?
 */
Image
read(const std::string &filename, std::stringstream *ss = nullptr);

/**
 * CCfits helper function
 * See http://heasarc.gsfc.nasa.gov/fitsio/ccfits/html/cookbook.html
 *
 * TODO: Add/adapt unit tests? -> Load, save and load -> compare...
 * TODO: Improve error handling?
 * TODO: Add write(const std::unique_ptr<ImageT> ...) ?
 *
 */
void
write(const cimg_library::CImg<uint16_t> &input_image, const std::string &filename, bool override = false,
      std::stringstream *ss = nullptr);

}
;
// namespace starmathpp::io::fits

#endif // STARMATHPP_CIMG_FITS_IO_H_

