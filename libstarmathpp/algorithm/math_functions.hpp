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

#ifndef STARMATHPP_ALGORITHM_MATH_FUNCTIONS_HPP_
#define STARMATHPP_ALGORITHM_MATH_FUNCTIONS_HPP_ STARMATHPP_ALGORITHM_MATH_FUNCTIONS_HPP_

#include <cmath>

namespace starmathpp::algorithm {

/**
 * Gaussian function
 */
template<typename T>
T gaussian(T x, T b, T p, T c, T w) {
  T t = ((x - c) / w);
  t *= t;

  return b + p * std::exp(-0.5f * t);
}

/**
 * Hyperbolic function
 * NOTE: Function only returns the positive part.
 */
template<typename T>
T hyperbolic(T x, T a, T b, T c, T d) {
  const T a2 = a * a;
  const T diff = x - c;
  const T diff2 = diff * diff;
  const T phi = std::sqrt(1.0 + (diff2 / a2));

  return b * phi + d;
}

/**
 * Inverse hyperbolic function
 * NOTE: Function only returns the positive part.
 *
 * TODO: Check that value under sqrt does not get < 0...
 *       currently we just return 0... is that ok?
 */
template<typename T>
T hyperbolic_inv(T x, T a, T b, T c, T d) {
  const T b2 = b * b;
  const T diff = x - d;
  const T diff2 = diff * diff;
  const T diff3 = (diff2 / b2) - 1.0;
  const T phi = (diff3 >= 0.0F ? std::sqrt(diff3) : 0.0F);

  return a * phi + c;
}

/**
 * Parabolic function
 */
template<typename T>
static T parabolic(T x, T a, T b, T c) {
  T x2 = x * x;
  return a * x2 + b * x + c;
}

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_MATH_FUNCTIONS_HPP_
