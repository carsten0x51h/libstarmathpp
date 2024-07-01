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

#ifndef STARMATHPP_IMAGE_READER_H
#define STARMATHPP_IMAGE_READER_H STARMATHPP_IMAGE_READER_H

#include <memory>

#include <libstarmathpp/io/filesystem_wrapper.hpp>
#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/image.hpp>

namespace starmathpp::io {
/**
 * Define ImageReaderException
 */
DEF_Exception(ImageReader);

/**
 * TODO: Document...
 *
 * @param filepath
 * @return
 */
Image read(const std::filesystem::path &filepath);
}  // namespace starmathpp

#endif // STARMATHPP_IMAGE_READER_H
