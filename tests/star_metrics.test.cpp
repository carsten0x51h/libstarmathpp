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

// Shared lib
// This is much faster than the header only variant
#define BOOST_TEST_MODULE "star metrics integration test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <tuple>
#include <vector>
#include <cmath>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/core.hpp>   // ranges::front()
#include <range/v3/algorithm/for_each.hpp>

#include <libstarmathpp/views/files.hpp>
#include <libstarmathpp/views/read.hpp>
#include <libstarmathpp/views/write.hpp>
#include <libstarmathpp/views/subtract_background.hpp>
#include <libstarmathpp/views/scale.hpp>
#include <libstarmathpp/views/center_on_star.hpp>
#include <libstarmathpp/views/crop.hpp>
//#include <libstarmathpp/pipeline/views/interpolate_bad_pixels.hpp>

#include <libstarmathpp/io/filesystem_wrapper.hpp>

#include <libstarmathpp/algorithm/threshold/otsu_thresholder.hpp>
#include <libstarmathpp/algorithm/centroid/intensity_weighted_centroider.hpp>
#include <libstarmathpp/algorithm/snr.hpp>
#include <libstarmathpp/algorithm/hfd.hpp>
#include <libstarmathpp/algorithm/fwhm.hpp>

#include <libstarmathpp/floating_point_equality.hpp>
#include <libstarmathpp/size.hpp>

BOOST_AUTO_TEST_SUITE (star_metrics_integration_tests)

using namespace ranges;
using namespace starmathpp;
using namespace starmathpp::io;
using namespace starmathpp::algorithm;
using namespace starmathpp::pipeline::views;

/**
 *
 */
bool are_tuples_equal(const std::tuple<float, float, float> &t1,
                      const std::tuple<float, float, float> &t2,
                      float tolerance) {
  return std::fabs(std::get<0>(t1) - std::get<0>(t2)) < tolerance
      && std::fabs(std::get<1>(t1) - std::get<1>(t2)) < tolerance
      && std::fabs(std::get<2>(t1) - std::get<2>(t2)) < tolerance;
}

/**
 * TODO: Add interpolate bad pixels...
 * TODO: saturated filter as function like add/sub, ...?
 * TODO: Implement simple crop().
 * TODO / PROBLEM: This way we lose the info which image we talk about... -> include path into ImageT structure...?!
 *                 -> This would also be helpful with a "store()/save()" function.
 *
 * TODO: Interesting observation: SNR gets worse again whens tar gets more into focus. Why? Is that expected?
 * TODO: Move HfdT to starmath::metrics::Hfd
 * TODO: To be changed to HfdT::calculate(...)? Or does the objet hold some valuable additional infos - like getOuterRadius()... which will now depend on the image input!
 *       FwhmT fwhmObj(ImageSlicerT::slice<SliceDirectionT::VERT>(*result, result->width()/2));
 * TODO: Change: pass in entire image -> calculate mean of VERT + HORZ Fwhm and return this as value. FwhmT should provide functions to get both separately.
 */
BOOST_AUTO_TEST_CASE(pipeline_star_metrics_test, * boost::unit_test::tolerance(0.5)) {
  std::vector<std::string> image_paths {
      "test_data/integration/star_metrics/newton_focus_star1.tiff",
      "test_data/integration/star_metrics/newton_focus_star2.tiff",
      "test_data/integration/star_metrics/newton_focus_star3.tiff",
      "test_data/integration/star_metrics/newton_focus_star4.tiff",
      "test_data/integration/star_metrics/newton_focus_star5.tiff",
      "test_data/integration/star_metrics/newton_focus_star6.tiff",
      "test_data/integration/star_metrics/newton_focus_star7.tiff",
      "test_data/integration/star_metrics/newton_focus_star8.tiff",
      "test_data/integration/star_metrics/newton_focus_star9.tiff",
      "test_data/integration/star_metrics/newton_focus_star10.tiff",
      "test_data/integration/star_metrics/newton_focus_star11.tiff" };

  std::vector<std::tuple<double, double, double>> expected_star_metrics = { {
      3.4248103734966566, 27.295144884427497, 7.0224903119455284 }, {
      4.0168380799027004, 25.431775505128925, 3.2577025906038513 }, {
      4.2398086088788904, 23.906962737212321, 4.9519681972932634 }, {
      4.5106426505897472, 21.797925759551219, 3.2653476572926494 }, {
      5.9366687230635415, 18.646887642682195, 5.1113245564233942 }, {
      5.9214817393646637, 15.900437967399453, 9.7376782809402265 }, {
      7.2058655558692744, 12.081075787803762, 3.7265062056625 }, {
      6.7269579863995439, 9.182079739644923, 4.978652931246689 }, {
      7.4242741594713344, 6.2974607834559153, 3.842207327064898 }, {
      5.278791273812149, 3.8438485006311049, 2.4601451100132845 }, {
      4.1407320775829746, 3.4434783057891602, 1.8549140611311068 } };

  auto star_metrics = image_paths | read()
//  | view::filter(& metrics::is_not_saturated)
      | subtract_background(OtsuThresholder<float>(16)) | scale_up(3.0F)
      | center_on_star(IntensityWeightedCentroider<float>()) | scale_down(3.0F)
      | crop_from_center(Size<int>(61, 61))
      | view::transform(
          [](const auto &img) {
            return std::make_tuple(
                starmathpp::algorithm::snr(img),
                starmathpp::algorithm::hfd(img),
                starmathpp::algorithm::fwhm(img).value());
          }) | to<std::vector>();

  float tolerance = 0.001F;

  BOOST_REQUIRE_EQUAL(star_metrics.size(), expected_star_metrics.size());

  for (size_t i = 0; i < star_metrics.size(); ++i) {
    BOOST_CHECK(
        are_tuples_equal(star_metrics[i], expected_star_metrics[i], tolerance));
  }
}

BOOST_AUTO_TEST_SUITE_END();
