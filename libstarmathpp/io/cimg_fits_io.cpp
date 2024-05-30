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

#include <CCfits/CCfits>
#include <memory>

#include <libstarmathpp/io/cimg_fits_io.hpp>

namespace starmathpp::io::fits {

/**
 * NOTE:
 * ends_with() is defined in C++20 and may be used here once all supported OS versions have C++20 support.
 *
 * See https://en.cppreference.com/w/cpp/string/basic_string/ends_with
 *
 */
bool is_fits(const std::string &filepath_lower) {
  return (boost::algorithm::ends_with(filepath_lower, "fit")
      || boost::algorithm::ends_with(filepath_lower, "fits"));
}

bool is_fits_gz(const std::string &filepath_lower) {
  return (boost::algorithm::ends_with(filepath_lower, "fit.gz")
      || boost::algorithm::ends_with(filepath_lower, "fits.gz"));
}

std::shared_ptr<Image> read(const std::string &filename,
                            std::stringstream *ss) {

  CCfits::FITS::setVerboseMode(ss != nullptr);

  try {
    // TODO: Memory leak? new....
    std::unique_ptr < CCfits::FITS
        > pInfile(new CCfits::FITS(filename, CCfits::Read, true));
    CCfits::PHDU &fits_img = pInfile->pHDU();

    // read all user-specific, coordinate, and checksum keys in the image
    fits_img.readAllKeys();

    if (ss != nullptr) {
      *ss << fits_img << std::endl;
    }

    auto img = std::make_shared < Image
        > ((int) fits_img.axis(0), (int) fits_img.axis(1));

    // TODO: Put a check here:   fitsImg.bitpix() <= sizeof(ImageT)
    // -> When provided image type does not fit, an exception is thrown? Or a warning logged?
    // Maybe exception is optional i.e. can be configured?
    //std::cerr << "fitsImg.bitpix(): " << fitsImg.bitpix() << std::endl;
    //std::cerr << "sizeof(ImageT): " << 8 * sizeof(typename Image::value_type)
    //        << std::endl;

    // HACK / FIXME: At this point we assume that there is only 1 layer!
    std::valarray<typename Image::value_type> img_data;
    fits_img.read(img_data);

    // For now we create a copy... maybe there is a better way to directly read data into CImg, later...
    cimg_forXY(*img, x, y)
    {
      // TODO: Should this be parameterized? Or is it possible to find out automatically?
      // Correct, when reading old, existing FITS files
      // NOTE: ImageJ and Gimp both work this way for normal files.
      //       -> For INDI/BLOB there must be a different solution.
      (*img)(x, img->height() - 1 - y) = img_data[img->offset(x, y)];

      // Correct when reading the image directly after storing the BLOB file with INDI.
      //(*outImg)(x, y) = imgData[outImg->offset(x, y)];
    }

    return img;

  } catch (CCfits::FitsException &exc) {
    throw FitsIOException(exc.message());
  }

  return nullptr;
}

/**
 *
 */
void write(const cimg_library::CImg<uint16_t> &input_image, const std::string &filename, bool override,
           std::stringstream *ss) {
  // TODO: Is it possible to pass a stream?
  CCfits::FITS::setVerboseMode(ss != nullptr);

  try {
    long naxis = 2;
    long naxes[2] = { input_image.width(), input_image.height() };

    std::unique_ptr < CCfits::FITS > pFits;

    // NOTE: The "!" should automatically override an existing file...
    //       See https://heasarc.gsfc.nasa.gov/fitsio/ccfits/html/writeimage.html
    std::string filepath = (override ? "!" : "") + filename;

    if (ss != nullptr) {
      *ss << "starmath::io::fits::write() writes to file '" << filepath << "'."
          << std::endl;
    }

    pFits = std::make_unique < CCfits::FITS
        > (filepath, USHORT_IMG, naxis, naxes);

    // NOTE: At this point we assume that there is only 1 layer.
    long nelements = std::accumulate(&naxes[0], &naxes[naxis], 1,
                                     std::multiplies<>());

    std::valarray<typename Image::value_type> array(nelements);

    cimg_forXY(input_image, x, y)
    {
      array[input_image.offset(x, y)] = input_image(x, input_image.height() - y - 1);
    }

    long fpixel(1);
    pFits->pHDU().write(fpixel, nelements, array);
  } catch (CCfits::FitsException &exc) {
    throw FitsIOException(exc.message());
  }
}

}  // starmathpp::io::fits
