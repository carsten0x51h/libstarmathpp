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
#define BOOST_TEST_MODULE "pipeline view crop unit test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <memory>

#include <boost/test/unit_test.hpp>

#include <range/v3/range/conversion.hpp>

#include <libstarmathpp/pipeline/views/crop.hpp>
#include <libstarmathpp/image.hpp>

BOOST_AUTO_TEST_SUITE (pipeline_crop_tests)

using namespace starmathpp;
using namespace starmathpp::pipeline;
using namespace starmathpp::pipeline::views;
using namespace ranges;

/**
 * This function generates a test-image.
 */
static ImagePtr generate_test_image(unsigned int width, unsigned int height,
                                    unsigned int test_pixel_pos_x,
                                    unsigned int test_pixel_pos_y,
                                    float bg_pixel_value,
                                    float test_pixel_value) {

  auto test_pixel_image_ptr = std::make_shared<Image>(width, height, 1, 1,
                                                     bg_pixel_value);

  (*test_pixel_image_ptr)(test_pixel_pos_x, test_pixel_pos_y) = test_pixel_value;

  return test_pixel_image_ptr;
}

/**
 * Test cropping a 3x3 rectangle from a 5x5 image with a
 * brihgt bixel in the center.
 */
BOOST_AUTO_TEST_CASE(pipeline_crop_from_center_sub_region_test)
{

  auto input_image = generate_test_image(25, 25,
      12, 12,
      250, 65535.0F);

  Image expected_result_image(3, 3, 1, 1, 250);   // 3x3 - bg value 250
  expected_result_image(1,1) = 65535.0F;       // Bright pixel at the center

  auto result_images = view::single(input_image)
      | crop_from_center(Size<int>(3, 3)) | to<std::vector>();

  // NOTE: Exactly one image is expected
  BOOST_TEST(result_images.size() == 1);
  BOOST_TEST(*(result_images.at(0)) == expected_result_image);
}

///**
// * Test cropping a 5x5 rectangle from a 5x5 image with a
// * bright pixel in the center.
// */
//BOOST_AUTO_TEST_CASE(pipeline_crop_from_center_full_image_test)
//{
//    ImageT expectedResultImage(5,5,1,1,250); // 5x5 - bg value 250
//    expectedResultImage(2,2) = 65535.0F;   // Bright pixel at the center
//
//    const std::vector<std::string> imageFilenames {
//            "test_data/image_processing_pipeline/crop/test_image_crop_from_center_5x5.tiff",
//    };
//
//    auto resultImagePtr = imageFilenames
//                          | read()
//                          | crop_from_center(Size<int>(5,5))
//                          | to<std::vector>();
//
//    // NOTE: Exactly one image is expected
//    BOOST_TEST(*(resultImagePtr.at(0)) == expectedResultImage);
//}
//
//
///**
// * range<image>   -->  crop(rects)  --> range < range <image> >
// *       ^                                          ^
// *       |                                          |
// *  input image                         range of cropped images
// *
// *  Per input image, N cropped images (one per rect).
// */
//BOOST_AUTO_TEST_CASE(pipeline_multi_crop_on_image_test)
//{
//	std::vector<Rect<int>> rects = { Rect<int>(0,0,10,10), Rect<int>(11,11,10,10) };
//
//	// TODO: Choose an input image from the crop test folder!
//    auto croppedImages =
//	  view::single("test_data/image_processing_pipeline/real_world/star_recognizer/test_image_star_recognizer_1.fit.gz")
//	  | read()
//	  | crop(rects)
//	  | to<std::vector>();
//
//    BOOST_TEST(croppedImages.size() == 1);       // One image goes in, one result is produced (which wraps a vector)
////    BOOST_TEST(croppedImages.at(0).size() == 2); // Two rects as input produce two output images
//}
//
///**
// * range<image>   -->  crop(rect)  --> range <image>
// *        ^                                     ^
// *        |                                     |
// *   input image                          cropped image
// *
// * Per input image, one cropped image.
// */
//BOOST_AUTO_TEST_CASE(pipeline_crop_on_image_test)
//{
//	Rect<int> rect1 = Rect<int>(0,0,10,10);
//
//	// TODO: Choose an input image from the crop test folder! Choose 2 images ... at least...
//    auto croppedImages =
//	  view::single("test_data/image_processing_pipeline/real_world/star_recognizer/test_image_star_recognizer_1.fit.gz")
//	  | read()
//	  | crop(rect1)
//	  | to<std::vector>();
//
//	std::cerr << "N crops from N images... N=" << croppedImages.size() << std::endl;
//
//	BOOST_TEST(croppedImages.size() == 1); // One image goes in, one result is produced
//}
//
//
//// TODO: Add crop_from_center test for even sized regions.
//// TODO: Test specified region exceeding the image dimensions.
//
//// TODO: Add "crop()" test

BOOST_AUTO_TEST_SUITE_END();
