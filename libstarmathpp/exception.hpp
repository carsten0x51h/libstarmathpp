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

#ifndef SOURCE_STARMATHPP_EXCEPTION_HPP_
#define SOURCE_STARMATHPP_EXCEPTION_HPP_ SOURCE_STARMATHPP_EXCEPTION_HPP_

#include <sstream>
#include <string>
#include <utility>

class BaseException : public std::exception {
 public:
  explicit BaseException(std::string name = "", std::string msg = "")
      :
      name_(std::move(name)),
      msg_(std::move(msg)) {
  }

  ~BaseException() noexcept override = default;

  [[nodiscard]] const char* what() const noexcept override {
    return msg_.c_str();
  }

 private:
  std::string name_;
  std::string msg_;
};

#define DEF_Exception(XYZ) \
  class XYZ##Exception : public BaseException {            \
  public:                                \
      explicit inline XYZ##Exception(const std::string & inMsg = "") : BaseException(#XYZ"Exception", inMsg) {} \
  }

#endif /* SOURCE_STARMATHPP_EXCEPTION_HPP_ */
