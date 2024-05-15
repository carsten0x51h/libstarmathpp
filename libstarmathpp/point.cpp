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

#include <libstarmathpp/point.hpp>

/**
 * Specialization for floating point values.
 *
 * @param p1
 * @param p2
 * @return
 */
template<>
bool operator==(const Point<float> &p1, const Point<float> &p2) {
  float dx = std::fabs(p1.x() - p2.x());
  float dy = std::fabs(p1.y() - p2.y());

  return (dx <= std::numeric_limits<float>::epsilon()
      && dy <= std::numeric_limits<float>::epsilon());
}

/**
 * Specialization for double values.
 *
 * @param p1
 * @param p2
 * @return
 */
template<>
bool operator==(const Point<double> &p1, const Point<double> &p2) {
  return (std::fabs(p1.x() - p2.x()) <= std::numeric_limits<double>::epsilon()
      && std::fabs(p1.y() - p2.y()) <= std::numeric_limits<double>::epsilon());
}
