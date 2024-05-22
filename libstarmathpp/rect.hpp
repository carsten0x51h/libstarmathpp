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

#ifndef SOURCE_STARMATHPP_RECT_HPP_
#define SOURCE_STARMATHPP_RECT_HPP_ SOURCE_STARMATHPP_RECT_HPP_

#include <type_traits>

#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/point.hpp>

//#include "size.h"

namespace starmathpp {

DEF_Exception(Rect);

/**
 * See https://stackoverflow.com/questions/16377736/stdmake-signed-that-accepts-floating-point-types
 */
template<typename T>
struct identity {
  using type = T;
};

template<typename T>
using try_make_unsigned =
typename std::conditional<
std::is_integral<T>::value,
std::make_unsigned<T>,
identity<T>
>::type;

template<typename T> class Rect;
// pre-declare the template class itself
template<typename T> std::ostream& operator<<(std::ostream &os,
                                              const Rect<T> &rect);

/**
 * Round differently depending on the specified return type.
 *
 * TODO: This might go to a separate header file.
 */
template<typename R>
typename std::enable_if<std::is_same<R, unsigned int>::value, unsigned int>::type round(
    double value) {
  return std::floor(value + 0.5);
}

template<typename R>
typename std::enable_if<std::is_same<R, int>::value, int>::type round(
    double value) {
  return (value < 0 ? std::ceil(value - 0.5) : std::floor(value + 0.5));
}

template<typename R>
typename std::enable_if<std::is_same<R, float>::value, float>::type round(
    double value) {
  return value;
}

template<typename R>
typename std::enable_if<std::is_same<R, double>::value, double>::type round(
    double value) {
  return value;
}

/**
 * Rect structure (X x Y x W x H).
 *
 * TODO: Is it a good idea to have an is_set_ state? Or better use std::optional of needed?
 * TODO: Restrict this template to data types which make sense.
 * TODO: is_set_ should be checked in all functions. If not set, and e.g. contains() is called, an exception should be thrown.
 * TODO: IDEA: Add contains(Point) + inside(Size?) +  contains(Size)?
 *
 */
template<class T>
class Rect {
 private:
  // Unsigned type, if possible
  using UT = typename try_make_unsigned<T>::type;

  T x_;
  T y_;
  UT width_;
  UT height_;
  bool is_set_;

 public:
  Rect()
      :
      x_(),
      y_(),
      width_(),
      height_(),
      is_set_(false) {
  }

  /**
   *
   */
  Rect(const T &x, const T &y, const UT &w, const UT &h)
      :
      x_(x),
      y_(y),
      width_(w),
      height_(h),
      is_set_(true) {
  }

  const T& x() const {
    return x_;
  }

  void set_x(const T &x) {
    x_ = x;
    is_set_ = true;
  }

  const T& y() const {
    return y_;
  }

  void set_y(const T &y) {
    y_ = y;
    is_set_ = true;
  }

  const UT& width() const {
    return width_;
  }

  /**
   *
   */
  void set_width(const UT &width) {
    width_ = width;
    is_set_ = true;
  }

  const UT& height() const {
    return height_;
  }

  /**
   *
   */
  void set_height(const UT &height) {
    height_ = height;
    is_set_ = true;
  }

  [[nodiscard]] bool is_set() const {
    return is_set_;
  }

  void clear() {
    *this = { };
  }

  template<class U>
  Rect<U> to() const {
    return Rect<U>(x_, y_, width_, height_);
  }

  /**
   * Returns true, if the passed rect completely contained inside this rect.
   * Of course both coordinates need to be in the same coordinate system.
   */
  bool contains(const Rect<T> &rect) {
    return rect1_contains_rect2_internal(*this, rect);
  }

  /**
   * Returns true, if this rect is completely inside the passed rect.
   * Of course both coordinates need to be in the same coordinate system.
   *
   * IDEA: Rename to is_inside()
   */
  bool inside(const Rect<T> &rect) {
    return rect1_contains_rect2_internal(rect, *this);
  }

  template<typename P = T>
  Point<P> center() const {
    return calc_center_from_rect_internal<P>(*this);
  }

  /**
   * IDEA: Alternate name: make_square
   */
  Rect<T> expand_to_square() const {
    auto center = calc_center_from_rect_internal<double>(*this);
    T sideLength = std::max(width_, height_);

    return from_center_point_internal<double, T, T>(center, sideLength,
                                                    sideLength);
  }

  /**
   * Grow rectangle by growBy in each direction.
   * NOTE: A negative value actually shrinks the Rect.
   */
  template<typename S>
  Rect<S> grow(S grow_by) const {
    return change_rect_size_internal(*this, grow_by, true /*grow*/);
  }

  /**
   * Shrink rectangle by shrinkBy in each direction.
   * NOTE: A negative value actually grows the Rect.
   */
  template<typename S>
  Rect<S> shrink(S shrink_by) const {
    return change_rect_size_internal<S>(*this, shrink_by, false /*shrink*/);
  }

  /**
   * Usage: Rect<>::from_center_point(point, width, height)
   */
  template<typename P, typename S>
  static Rect<T> from_center_point(const Point<P> &center, S width, S height) {
    return from_center_point_internal<P, S, T>(center, width, height);
  }

  // TODO: Re-enable?
  // static RectT<T>
  // from_center_point(const PointT<T> &inCenter, SizeT<T> inWindowSize) {
  //     return from_center_point_internal(inCenter, inWindowSize.width(), inWindowSize.height());
  // }

  /**
   *
   */
  template<typename P, typename S>
  static Rect<T> from_center_point(const Point<P> &center,
                                   S window_size_square_edge) {
    return from_center_point_internal<P, S, T>(center, window_size_square_edge,
                                               window_size_square_edge);
  }

 private:

  /**
   *
   */
  template<typename P, typename S, typename R>
  static Rect<R> from_center_point_internal(const Point<P> &center, S width,
                                            S height) {

    double delta_w = (double) width / 2.0;
    double delta_h = (double) height / 2.0;

    return Rect<R>(round<R>((double) center.x() - delta_w),
                   round<R>((double) center.y() - delta_h), width, height);
  }

  /**
   * Internal helper function to clip a value to 0.
   */
  template<typename S>
  static S clip_to_zero(S value) {
    return (value < 0 ? 0 : value);
  }

  /**
   *
   */
  template<typename S>
  static Rect<S> change_rect_size_internal(Rect<T> rect, S change_by,
                                           bool grow) {
    auto center = calc_center_from_rect_internal<float>(rect);

    S border_both_sides = 2.0 * change_by;

    S new_width = (
        grow ?
            (S) (rect.width() + border_both_sides) :
            (S) clip_to_zero((S) rect.width() - border_both_sides));

    S new_height = (
        grow ?
            (S) (rect.height() + border_both_sides) :
            (S) clip_to_zero((S) rect.height() - border_both_sides));

    return from_center_point_internal<float, S, S>(center, new_width,
                                                   new_height);
  }

  /**
   *
   */
  template<typename P>
  static Point<P> calc_center_from_rect_internal(const Rect<T> &rect) {
    if (!rect.is_set_) {
      throw RectException("Cannot perform operation on empty Rect.");
    }

    P x_center = rect.x() + rect.width() / 2.0;
    P y_center = rect.y() + rect.height() / 2.0;

    return Point<P>(x_center, y_center);
  }

  // TODO: Does not work in case of float!!!
  static bool is_even(int n) {
    return (n % 2 == 0);
  }

  // Is rect1 completely contained inside this rect2?
  // Of course both coordinates need to be in the same coordinate system.
  static bool rect1_contains_rect2_internal(const Rect<T> &rect1,
                                            const Rect<T> &rect2) {

    if (!rect1.is_set_ || !rect2.is_set_) {
      return false;
    }

    bool contains = rect2.x() >= rect1.x() && rect2.y() >= rect1.y()
        && rect2.x() + rect2.width() <= rect1.x() + rect1.width()
        && rect2.y() + rect2.height() <= rect1.y() + rect1.height();

    return contains;
  }
};

template<class T>
std::ostream& operator<<(std::ostream &os, const Rect<T> &rect) {

  os << "(x=" << rect.x() << ", y=" << rect.y() << ", width=" << rect.width()
      << ", height=" << rect.height() << ")";

  return os;
}

template<class T>
bool operator==(const Rect<T> &rect1, const Rect<T> &rect2) {
  return rect1.x() == rect2.x() && rect1.y() == rect2.y()
      && rect1.width() == rect2.width() && rect1.height() == rect2.height();
}

// Template specialization for float
template<>
bool operator==<float>(const Rect<float> &rect1, const Rect<float> &rect2);

// Template specialization for double
template<>
bool operator==<double>(const Rect<double> &rect1, const Rect<double> &rect2);

template<class T>
bool operator!=(const Rect<T> &rect1, const Rect<T> &rect2) {
  // Use implementation of operator== and negate it.
  return !(rect1 == rect2);
}

}  // namespace starmathpp

#endif // SOURCE_STARMATHPP_RECT_HPP_
