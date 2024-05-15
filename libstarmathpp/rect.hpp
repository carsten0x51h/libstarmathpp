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

#include <tuple>

#include <libstarmathpp/point.hpp>

//#include "size.h"
//#include "logging.h"
//#include "exception.h"
//#include "tuple_printer.h"

//DEF_Exception(Rect);

/**
 * Rect structure (X x Y x W x H).
 * TODO: Overload operator== and operator!=
 * TODO: Overload operator<<
 * TODO: Put this fully under unit tests for different data types.
 * TODO: Restrict this template to data types which make sense.
 * TODO / IDEA: Make w & h as unsigned for fixed point types and x & y as signed.
 * TODO: is_set_ should be checked in all functions. If not set, and e.g. contains() is called, an exception should be thrown.
 */
template<class T>
class Rect {
 private:
  T x_;
  T y_;
  T width_;
  T height_;
  bool is_set_;

 public:
  Rect()
      :
      is_set_(false) {
  }

  Rect(const T &x, const T &y, const T &w, const T &h)
      :
      x_(x),
      y_(y),
      width_(w),
      height_(h),
      is_set_(true) {
  }

  template<class U>
  Rect<U> to() const {
    return Rect<U>(x_, y_, width_, height_);
  }

  const T& x() const {
    return x_;
  }

  void set_x(const T &x) {
    is_set_ = true;
    x_ = x;
  }

  const T& y() const {
    return y_;
  }

  void set_y(const T &y) {
    y_ = y;
    is_set_ = true;
  }

  const T& width() const {
    return width_;
  }

  void set_width(const T &width) {
    width_ = width;
    is_set_ = true;
  }

  const T& height() const {
    return height_;
  }

  void set_height(const T &height) {
    height_ = height;
    is_set_ = true;
  }

  [[nodiscard]] bool is_set() const {
    return is_set_;
  }

  void clear() {
    is_set_ = false; // TODO: Is setting is_set_ to false even required?
    *this = { };
  }  // TODO:  {} ok??

  // Is passed rect completely contained inside this rect?
  // Of course both coordinates need to be in the same coordinate system.
  bool contains(const Rect<T> &rect) {
    return rect1_contains_rect2_internal(*this, rect);
  }

//	TODO: operator= to allow assignment from Rect<float> -> Rect<unsigned int> etc. ??

  // Is this rect completely inside the passed rect?
  // Of course both coordinates need to be in the same coordinate system.
  // TODO: Maybe rename to isInside()?
  bool inside(const Rect<T> &rect) {
    return rect1_contains_rect2_internal(rect, *this);
  }

  Point<T> center() const {
    return calc_center_from_rect_internal(*this);
  }

  /**
   * IDEA: Alternate name: make_square
   */
  Rect<T> expand_to_square() const {
    auto center = calc_center_from_rect_internal(*this);
    T sideLength = std::max(width_, height_);

    return from_center_point(center, sideLength);
  }

  /**
   * Grow rectangle by growBy in each direction.
   * TODO: A negative value should throw an exception...
   */
  Rect<T> grow(T grow_by) const {
    return change_rect_size_internal(*this, grow_by, true /*grow*/);
  }

  /**
   * Shrink rectangle by shrinkBy in each direction.
   * TODO: A negative value should throw an exception...
   */
  Rect<T> shrink(T shrink_by) const {
    return change_rect_size_internal(*this, shrink_by, false /*shrink*/);
  }

  // TODO: Idea: Add
  // 2. RectT::grow(3)
  // 3. RectT::shrink(3)

  static Rect<T> from_center_point(const Point<T> &center, T width,
                                  T height) {
    // TODO: Or should we round here to int? Check if -1 is correct...
    //unsigned int halfWindowWidth = ceil(inWidth / 2.0f) - 1;
    //unsigned int  halfWindowHeight = ceil(inHeight / 2.0f) - 1;

    // TODO: Does not work in case of float!!!
    //            -> Implement for RectT and not for RectF... or tmpl specialization!
    //            -> Easiest: Only permit odd value!
//        if (isEven(inWidth) || isEven(inHeight)) {
    //           throw RectExceptionT("Window width and height expected to be odd values.");
    //       }

    float delta_w = ((float) width - 1.0F) / 2.0F;
    float delta_h = ((float) height - 1.0F) / 2.0F;

//	  T halfWindowWidth = inWidth / 2.0f;
//	  T halfWindowHeight = inHeight / 2.0f;
//	  return RectT<T>(std::get<0>(inCenter) /*cx*/ - halfWindowWidth, std::get<1>(inCenter) /*cy*/ - halfWindowHeight, inWidth, inHeight);

    return Rect<T>((T) ((float) center.x() - delta_w),
                    (T) ((float) center.y() - delta_h), width, height);
  }

  // TODO: Re-enable?
  // static RectT<T>
  // from_center_point(const PointT<T> &inCenter, SizeT<T> inWindowSize) {
  //     return from_center_point(inCenter, inWindowSize.width(), inWindowSize.height());
  // }
  // TODO: public functions should not call other public functions!
  static Rect<T> from_center_point(const Point<T> &center,
                                  T window_size_square_edge) {
    return from_center_point(center, window_size_square_edge,
                             window_size_square_edge);
  }

  // TODO: For float and double, operator needs to be specialized! See point...
  bool operator==(const Rect<T> &other) const {
    return other.x_ == this->x_ && other.y_ == this->y_
        && other.width_ == this->width_ && other.height_ == this->height_;
  }
  bool operator!=(const Rect<T> &other) const {
    return !this->operator==(other);
  }

  std::ostream& print(std::ostream &os, size_t indent = 0) const {
    std::string prefix = std::string(indent, ' ');

    os << prefix << "(x=" << x_ << ", y=" << y_ << ", width=" << width_
        << ", height=" << height_ << ")";

    return os;
  }

  friend std::ostream& operator<<(std::ostream &os, const Rect<T> &rect) {
    return rect.print(os);
  }

 private:
  // TODO: If increaseBy is negative, check that overall width & height cannot get negative.
  //        -> Limit values... 0? Or exception...
  static Rect<T> change_rect_size_internal(Rect<T> rect, T change_by, bool grow) {
    auto center = calc_center_from_rect_internal(rect);
    T border_both_sides = 2 * change_by;

    T new_width = (
        grow ?
            rect.width() + border_both_sides :
            rect.width() - border_both_sides);
    T new_height = (
        grow ?
            rect.height() + border_both_sides :
            rect.height() - border_both_sides);

    return from_center_point(center, new_width, new_height);
  }

  static Point<T> calc_center_from_rect_internal(const Rect<T> &rect) {
    T x_center = rect.x() + rect.width() / 2.0;
    T y_center = rect.y() + rect.height() / 2.0;

    return Point<T>(x_center, y_center);
  }

  // TODO: Does not work in case of float!!!
  static bool is_even(int n) {
    return (n % 2 == 0);
  }

  // Is rect1 completely contained inside this rect2?
  // Of course both coordinates need to be in the same coordinate system.
  static bool rect1_contains_rect2_internal(const Rect<T> &rect1, const Rect<T> &rect2) {

    //LOG(debug) << "rect1ContainsRect2 - rect1=" << rect1 << ", rect2=" << rect2 << std::endl;

    if (!rect1.is_set_ || !rect2.is_set_) {
      return false;
    }

    bool contains = rect2.x() >= rect1.x() && rect2.y() >= rect1.y()
        && rect2.x() + rect2.width() <= rect1.x() + rect1.width()
        && rect2.y() + rect2.height() <= rect1.y() + rect1.height();

    return contains;
  }

};

#endif /* SOURCE_STARMATHPP_RECT_HPP_ */
