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

#ifndef LIBSTARMATHPP_ALGORITHM_HPP_
#define LIBSTARMATHPP_ALGORITHM_HPP_

#include <libstarmathpp/algorithm/average.hpp>
#include <libstarmathpp/algorithm/bad_pixel_median_interpolator.hpp>
#include <libstarmathpp/algorithm/fwhm.hpp>
#include <libstarmathpp/algorithm/hfd.hpp>
#include <libstarmathpp/algorithm/snr.hpp>
#include <libstarmathpp/algorithm/star_cluster_algorithm.hpp>

#include <libstarmathpp/algorithm/centroid/centroider.hpp>
#include <libstarmathpp/algorithm/centroid/center_of_gravity_centroider.hpp>
#include <libstarmathpp/algorithm/centroid/intensity_weighted_centroider.hpp>

#include <libstarmathpp/algorithm/stretch/stretcher.hpp>
#include <libstarmathpp/algorithm/stretch/midtone_balance_stretcher.hpp>

#include <libstarmathpp/algorithm/threshold/thresholder.hpp>
#include <libstarmathpp/algorithm/threshold/max_entropy_thresholder.hpp>
#include <libstarmathpp/algorithm/threshold/mean_thresholder.hpp>
#include <libstarmathpp/algorithm/threshold/otsu_thresholder.hpp>

#endif /* LIBSTARMATHPP_ALGORITHM_HPP_ */
