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

#include <cmath> // std::abs()

#include <libstarmathpp/rect.hpp>

/**
 * Compare function for float and double.
 */
template<typename T>
bool compare(const Rect<T> &rect1, const Rect<T> &rect2) {
  T delta_x = std::abs(rect1.x() - rect2.x());
  T delta_y = std::abs(rect1.y() - rect2.y());
  T delta_width = std::abs(rect1.width() - rect2.width());
  T delta_height = std::abs(rect1.height() - rect2.height());

  return delta_x <= std::numeric_limits<T>::epsilon()
      && delta_y <= std::numeric_limits<T>::epsilon()
      && delta_width <= std::numeric_limits<T>::epsilon()
      && delta_height <= std::numeric_limits<T>::epsilon();
}


template<>
bool operator==(const Rect<float> &rect1, const Rect<float> &rect2) {
  return compare(rect1, rect2);
}


template<>
bool operator==(const Rect<double> &rect1, const Rect<double> &rect2) {
  return compare(rect1, rect2);
}
