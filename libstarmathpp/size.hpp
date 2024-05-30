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

#ifndef STARMATHPP_SIZE_HPP_
#define STARMATHPP_SIZE_HPP_ STARMATHPP_SIZE_HPP_

#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/point.hpp>

namespace starmathpp {

DEF_Exception(Size);

template<typename T> class Size;
// pre-declare the template class itself
template<typename T> std::ostream& operator<<(std::ostream &os,
                                              const Size<T> &size);

/**
 * Size structure (W x H).
 *
 * TODO: Introduce concept to allow only unsigned (positive) values for T.
 * TODO: Proper rounding in to<>() function.
 * TODO: shrink + grow? - Move functionality from Rect?
 *
 */
template<class T>
class Size {
 private:
  T width_;
  T height_;

 public:
  Size() {
  }

  Size(T width, T height)
      :
      width_(width),
      height_(height) {
  }

  T width() const {
    return width_;
  }

  void set_width(T width) {
    width_ = width;
  }

  T height() const {
    return height_;
  }

  void set_height(T height) {
    height_ = height;
  }

  template<class U>
  Size<U> to() const {
    return Size<U>(width_, height_);
  }

  Size<T> operator*(T factor) {
    return Size<T>(width_ * factor, height_ * factor);
  }
};

template<class T>
std::ostream& operator<<(std::ostream &os, const Size<T> &size) {

  os << "(width=" << size.width() << ", height=" << size.height() << ")";

  return os;
}

template<class T>
bool operator==(const Size<T> &size1, const Size<T> &size2) {
  return (size1.width() == size2.width()) && (size1.height() == size2.height());
}

//// Template specialization for float
//template<>
//bool operator==<float>(const Rect<float> &rect1, const Rect<float> &rect2);
//
//// Template specialization for double
//template<>
//bool operator==<double>(const Rect<double> &rect1, const Rect<double> &rect2);

template<class T>
bool operator!=(const Size<T> &size1, const Size<T> &size2) {
  // Use implementation of operator== and negate it.
  return !(size1 == size2);
}

}  // namespace starmathpp

#endif // STARMATHPP_SIZE_HPP_
