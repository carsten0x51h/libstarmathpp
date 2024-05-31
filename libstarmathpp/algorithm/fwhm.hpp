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

#include <cmath>
#include <algorithm>

#include <ceres/ceres.h>

#include <range/v3/view/iota.hpp>

#include <libstarmathpp/image.hpp>
#include <libstarmathpp/exception.hpp>
#include <libstarmathpp/point.hpp>

namespace starmathpp::algorithm {

DEF_Exception(Fwhm);

namespace detail {

/**
 * Define a cost functor for Gaussian curve fitting.
 * TODO: Use Point? -> No. Supplying vectors of data is more flexible - e.g. x, y and z data...
 */
struct GaussianResidual {
  GaussianResidual(double x, double y)
      :
      x_(x),
      y_(y) {
  }
  template<typename T>
  bool operator()(const T *const params, T *residual) const {
    T A = params[0];
    T mu = params[1];
    T sigma = params[2];
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
std::vector<ImageType> extract_row(
    const cimg_library::CImg<ImageType> &input_image, size_t row_idx) {
  // TODO: Checks...
  std::vector<float> row_values(input_image.width());
  auto row_img = input_image.get_row(row_idx);

  std::copy(row_img.begin(), row_img.end(), row_values.begin());
  return row_values;
}

/**
 *
 */
template<typename ImageType>
std::vector<ImageType> extract_col(
    const cimg_library::CImg<ImageType> &input_image, size_t col_idx) {
  // TODO: Checks...
  std::vector<float> col_values(input_image.height());
  auto col_img = input_image.get_column(col_idx);

  std::copy(col_img.begin(), col_img.end(), col_values.begin());
  return col_values;
}

/**
 * Calc FWHM ["] from FWHM [px]
 * FWHM = sigma * 1.66510922 (=2*sqrt(ln(2)))
 *
 * See http://mathworld.wolfram.com/GaussianFunction
 *
 * NOTE: std::log() is ln()
 * TODO: --> Needs F?! --> Move out of here?!--> Diff class?!
 * TODO: We want to have this value in PX and ".... depending on the purpose...
 */
static inline double sigma_to_fwhm(double sigma) {
  constexpr double SIGMA_TO_FWHM = 2.0 * std::sqrt(std::log(2.0));
  return SIGMA_TO_FWHM * sigma;
}

/**
 *
 */
template<typename ImageType>
std::vector<double> make_guess(const std::vector<ImageType> &input_values) {
  // TODO: THROW_IF(LmCurveMatcher, input_values.size() < 2, "input_values.size() < 2!")

  auto max_element_iter = std::max_element(input_values.begin(),
                                           input_values.end());
  return std::vector<double> { *max_element_iter, /* max. y value -> A*/
  (double) std::distance(input_values.begin(), max_element_iter), /*x value of max. y value -> mu */
  input_values.size() / 10.0 /* "x-range" of values divided by 10 */
  };
}

/**
 * TODO: For bad start values (e.g. { 5.0, 3.0, 1.0 }),
 *       the solver still returns CONVERGENCE, but the result
 *       appears to be wrong -> check! Converged criteria?
 *       -> rel/abs. error?
 *       -> Add unit tests to test error cases!
 *
 * TODO: AddResidualBlock allows passing std::vector<double*>...
 *       -> avoid copy below?
 */
template<typename RNG, typename ImageType>
double fwhm_1d_internal(RNG x_data, const std::vector<ImageType> &y_data) {

  // Initial guess for parameters A, mu, sigma
  auto params_vec = make_guess(y_data);  //{ 5.0, 3.0, 1.0 };
  double params[3];
  std::copy(params_vec.begin(), params_vec.end(), params);

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

//  google::SetStderrLogging(google::FATAL);
  // See https://stackoverflow.com/questions/21410982/ceres-solver-unable-to-disable-logging-google-glog
  options.logging_type = ceres::SILENT;  // TODO: Bind to debug...? Switch?
  options.minimizer_progress_to_stdout = false;  // TODO: Bind to debug...? Switch?

  // Solve the problem
  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);

  // Output the results
//  std::cout << summary.FullReport() << "\n";
  std::cout << "Success? " << (summary.termination_type == ceres::CONVERGENCE)
      << std::endl;

  // TODO: Only do the stuff below if fitting was a success...
  double sigma = std::abs(params[2]);

  std::cout << "Estimated parameters: A = " << params[0] << ", mu = "
      << params[1] << ", sigma = " << sigma << "   -> ";

  // See https://stackoverflow.com/questions/47773178/gaussian-fit-returning-negative-sigma
  return sigma_to_fwhm(sigma);
}

/**
 * TODO: Extract calculation of FWHM value for (x_data, y_data)
 *       and then call it twice for each "direction"...
 *
 * TODO: Conversion from star_center.y() to pixel/idx coordinates...
 */
template<typename ImageType>
double fwhm_internal(const cimg_library::CImg<ImageType> &input_image,
                     const Point<float> &star_center, float scale_factor) {

  if (input_image.is_empty()) {
    throw FwhmException("Empty image supplied.");
  }

  double fwhm_horizontal = fwhm_1d_internal(
      ranges::view::ints(0, input_image.width()),
      extract_row(input_image, star_center.y())  // horizontal_slice
                  );

  double fwhm_vertical = fwhm_1d_internal(
      ranges::view::ints(0, input_image.height()),
      extract_col(input_image, star_center.x())  // vertical_slice
                  );

  return (fwhm_horizontal + fwhm_vertical) / 2.0;
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
