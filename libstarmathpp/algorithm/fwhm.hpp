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

#ifndef STARMATHPP_ALGORITHM_FWHM_HPP_
#define STARMATHPP_ALGORITHM_FWHM_HPP_ STARMATHPP_ALGORITHM_FWHM_HPP_

#include <ceres/ceres.h>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/point.hpp>

namespace starmathpp::algorithm {

DEF_Exception(Fwhm);

namespace detail {

/**
 * Define a cost functor for Gaussian curve fitting.
 */
struct GaussianResidual {
  GaussianResidual(double x, double y)
      :
      x_(x),
      y_(y) {
  }
  template<typename T>
  bool operator()(const T *const params, T *residual) const {
    T A = params[0];  // p
    T mu = params[1];  // c
    T sigma = params[2];  // w
    residual[0] = y_ - A * exp(-0.5 * pow((x_ - mu) / sigma, 2));
    return true;
  }
 private:
  const double x_;
  const double y_;
};

/**
 *
 */
template<typename ImageType>
double fwhm_internal(const cimg_library::CImg<ImageType> &input_image,
                     const Point<float> &star_center, float scale_factor) {

  if (input_image.is_empty()) {
    throw FwhmException("Empty image supplied.");
  }

  // Sample data points (x, y)
  std::vector<double> x_data = { 1, 2, 3, 4, 5 };
  std::vector<double> y_data { 2.5, 3.5, 5.0, 3.5, 2.5 };

  // Initial guess for parameters A, mu, sigma
  double params[3] = { 5.0, 3.0, 1.0 };

  // Build the problem
  ceres::Problem problem;

  for (size_t i = 0; i < x_data.size(); ++i) {
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<GaussianResidual, 1, 3>(
            new GaussianResidual(x_data[i], y_data[i])),
        nullptr, params);
  }

  // Configure the solver
  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;

  // Solve the problem
  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);

  // Output the results
  std::cout << summary.FullReport() << "\n";
  std::cout << "Estimated parameters: A = " << params[0] << ", mu = "
      << params[1] << ", sigma = " << params[2] << "\n";

  return 123.456; // TODO: FIXME
}
}  // namespace detail

/*+
 *
 */
template<typename ImageType>
double fwhm(const cimg_library::CImg<ImageType> &input_image,
            const Point<float> &star_center, float scale_factor = 1.0F) {

  return detail::fwhm_internal(input_image, star_center, scale_factor);
}

/*+
 *
 */
template<typename ImageType>
double fwhm(const cimg_library::CImg<ImageType> &input_image,
            float scale_factor = 1.0F) {

  Point<float> star_center((float) input_image.width() / 2,
                           (float) input_image.height() / 2);

  return detail::fwhm_internal(input_image, star_center, scale_factor);
}

}  // namespace starmathpp::algorithm

#endif // STARMATHPP_ALGORITHM_FWHM_HPP_
