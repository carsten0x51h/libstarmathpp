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

#ifndef SOURCE_STARMATHPP_POINT_HPP_
#define SOURCE_STARMATHPP_POINT_HPP_ SOURCE_STARMATHPP_POINT_HPP_

#include <limits>
#include <cmath>
#include <ostream>
#include <tuple>

namespace starmathpp {

template<typename T> class Point;
// pre-declare the template class itself
template<typename T> std::ostream& operator<<(std::ostream &os,
                                              const Point<T> &p);

/**
 * PointT structure (X x Y).
 */
template<class T>
class Point {
 public:
  Point()
      :
      x_(0),
      y_(0) {
  }

  Point(const T &x, const T &y)
      :
      x_(x),
      y_(y) {
  }

  [[nodiscard]] const T& x() const {
    return x_;
  }

  void set_x(const T &x) {
    x_ = x;
  }

  [[nodiscard]] const T& y() const {
    return y_;
  }

  void set_y(const T &y) {
    y_ = y;
  }

  template<class U>
  Point<U> to() const {
    return Point<U>(x_, y_);
  }

 private:
  T x_;
  T y_;
};

template<class T>
std::ostream& operator<<(std::ostream &os, const Point<T> &p) {
  os << "(" << p.x() << "," << p.y() << ")";
  return os;
}

template<class T>
bool operator==(const Point<T> &p1, const Point<T> &p2) {
  return (p1.x() == p2.x() && p1.y() == p2.y());
}

template<class T>
bool operator!=(const Point<T> &p1, const Point<T> &p2) {
  // Use implementation of operator== and negate it.
  return !(p1 == p2);
}

template<>
bool operator==<float>(const Point<float> &p1, const Point<float> &p2);

template<>
bool operator==<double>(const Point<double> &p1, const Point<double> &p2);

/**
 * Definition: One point is "bigger" than another when the distance to (0,0) is bigger.
 * NOTE: This might have a performance impact and may have to be changed...
 * TODO: Revisit this definition... crosscheck with std::tie below...
 *
 * @tparam T
 * @param p1
 * @param p2
 * @return
 */
template<class T>
bool operator<(const Point<T> &p1, const Point<T> &p2) {
  // An idiomatic way to implement strict weak ordering for a structure is to use lexicographical comparison provided by std::tie.
  // See https://en.cppreference.com/w/cpp/language/operators
  // NOTE: This has a dependency to <tuple>
  return (std::tie(p1.x(), p1.y()) < std::tie(p2.x(), p2.y()));
}

}  // end namespace starmathpp

#endif /* SOURCE_STARMATHPP_POINT_HPP_ */
