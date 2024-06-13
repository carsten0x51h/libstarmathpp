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
#define BOOST_TEST_MODULE "image development integration test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/single.hpp>
//#include <range/v3/view/join.hpp>
#include <range/v3/action/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/core.hpp>   // ranges::front()
#include <range/v3/algorithm/for_each.hpp>

#include <libstarmathpp/algorithm/threshold/otsu_thresholder.hpp>
#include <libstarmathpp/algorithm/centroid/intensity_weighted_centroider.hpp>

#include <libstarmathpp/pipeline/views/files.hpp>
#include <libstarmathpp/pipeline/views/read.hpp>
#include <libstarmathpp/pipeline/views/write.hpp>
#include <libstarmathpp/pipeline/views/interpolate_bad_pixels.hpp>
#include <libstarmathpp/pipeline/views/detect_stars.hpp>
#include <libstarmathpp/pipeline/views/scale.hpp>
#include <libstarmathpp/pipeline/views/crop.hpp>
#include <libstarmathpp/pipeline/views/center_on_star.hpp>

//#include <libstarmathpp/io/filesystem_wrapper.hpp>

#include <libstarmathpp/algorithm/average.hpp>

#include <libstarmathpp/floating_point_equality.hpp>

BOOST_AUTO_TEST_SUITE (star_recognizer_integration_tests)

using namespace ranges;
using namespace starmathpp;
using namespace starmathpp::io;
using namespace starmathpp::algorithm;
using namespace starmathpp::pipeline::views;

/**
 *      3. Cluster example (old automatic star analyzer)
 *
 *          List<img> imgs = AstroImageProcessingPipelineT
 *                              .of("my-star-image.fits")
 *                              .denoise(DenoiserT::...)
 *                              .subtractBackground(ThresholderT::otsu(...) OR thresholder function)
 *                              .cluster(ClusterAlgorithmT::...)                                          -> List<List<img>>
 *                              .flatMap()
 *                              .filter(StarDetailsT::isSaturated() == false)
 *                              .centerOnStar(CentroiderT::iwc(...))
 *                              .filter(StarDetailsT::hfd() < 2)   ??
 *                              .filter(StarDetailsT::snr() > 10)  ??
 *                              .collect(Collector::asList())
 *
 *          OR with range library
 *
 *          NOTE: flatmap: https://stackoverflow.com/questions/36051851/how-to-implement-flatmap-using-rangev3-ranges
 *
 *          images("my-star-image.fits")
 *              | denoise(DenoiserT::...())
 *              | subtractBackground(ThresholderT::otsu(...) OR thresholder function)
 *              | cluster(ClusterAlgorithmT::...)
 *              | boost::range::for_each() -> Range of images (NOTE: Images can have different sizes)
 *              | filtered(! StarAnalysisT::isSaturated())
 *              | centerOnStar(CentroiderT::iwc(...))
 *              | filtered(! StarAnalysisT::snr() > 10)
 *              | filtered(! StarAnalysisT::hfd() < 5)
 *
 * TODO: Why is pipeline processed twice? -> detet_stars() ... to<vector>()?...
 * TODO: Smaller input images with only 2-3 stars... but multiple ones...
 * TODO: Add hot-pixel removal before detection,,,, maybe also denoise()...
 * TODO / IDEA: Add example-program which generates an image where the detected stars are marked ... + HFD - like the original star-recognizer...
 * TODO: Add center_on_star() for each detected star image... then calc SnrT, HfdT and FwhmT....
 */
BOOST_AUTO_TEST_CASE(pipeline_star_recognizer_test) {
  // TODO: At least two files...
  auto detected_star_images =
      view::single(
          "test_data/integration/star_recognizer/test_image_star_recognizer_1.fit.gz")
          | read()
          | interpolate_bad_pixels(500 /*threshold*/, 3 /*filter size*/)
          | detect_stars(2 /*cluster radius*/, OtsuThresholder<float>(16), 30 /*border width*/
          ) | crop()
//        | views::join // crop() gives range<vector<img>, vector<img>, ...> This does not work with ubuntu 22.04 (older version of ranges v3),therefore, transform below...
          | view::transform(
              [](const auto &detectedStars /*std::vector<cimg_library::CImg<ImageType>> */) {
                return detectedStars | scale_up(3.0F)
                    | center_on_star(IntensityWeightedCentroider<float>())
                    | scale_down(3.0F)
                    | write<float>(std::filesystem::current_path(), "img_%04d.fit")  // NOTE; path must exist, TODO: directory should change for each input image...
                    | to<std::vector>();
              }) | actions::join | to<std::vector>();

//    BOOST_TEST(clusteredImagesRanges.size() == 1); // Should correspond to the number of input images
  BOOST_TEST(detected_star_images.size() == 92);// 216 detected stars (without hot-pixel removal)

  // Test; Calculate SNR and HFD
  // NOTE: Question is, if before HFD calculation, the background should be subtracted, or not....
  //       and if this should happen inside the HFD... No, because the HFD algorithm is
  //       independent from that. It is correct for both images - with and without background.
  //       Just the value it returns, is quite different.
//    auto starMetricsRange =
//        clusteredImagesRanges
//          | subtract_background(ThresholdingAlgorithmFactoryT::getInstance(ThresholdingAlgorithmTypeT::OTSU)) // TODO: Why not just passing in ThresholdingAlgorithmTypeT::OTSU?              | scale_up(3.0F)
//      | view::transform(
//          [](const auto & imgPtr) {
//                  return std::make_tuple(SnrT::calculate(*imgPtr), HfdT(*imgPtr).getValue());
//            });

//    ranges::for_each(starMetricsRange, [](const auto & starMetrics){
//        std::cout << "SNR: " << std::get<0>(starMetrics)
//            << ", HFD: " << std::get<1>(starMetrics)
//        << std::endl;
//    });

  // DEBUG START
  // Write result images...
//    int counter = 0;
//
//    for(const auto & starImg : clusteredImagesRanges) {
//      std::stringstream ss;
//      ss << "star_img" << counter++ << ".png";
//      std::cerr << "Storing " << ss.str() << "..." << std::endl;
//
//      starImg->save(ss.str().c_str());
//    }
  // DEBUG END

}

BOOST_AUTO_TEST_SUITE_END();
