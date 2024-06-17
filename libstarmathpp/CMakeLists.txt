include (AddUnitTest)

#
# Target name
#
set(target starmathpp)


#
# Generate version-header
#
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/version.hpp)


# 
# Sources
#
set(sources
   point.cpp
   rect.cpp
   io/cimg_fits_io.cpp
   io/image_reader.cpp
   io/image_writer.cpp
   algorithm/star_cluster_algorithm.cpp
)


# 
# Build library
#
add_library(${target} ${sources})


# Other libraries the starmathpp library depends on
target_link_libraries(${target}
	INTERFACE
	${TIFF_LIBRARIES}
	${PNG_LIBRARIES}
	${JPEG_LIBRARIES}
	${CERES_LIBRARIES}
	glog::glog
	PRIVATE
	${CFITSIO_LIBRARY}
	${CCFITS_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
)


if(DEBUG_IMAGE_DISPLAY_SWITCH)
   message("* Pipeline debug is ENABLED. Adding X11 dependency.")
   find_package (X11 REQUIRED)
   target_link_libraries(${target}
	INTERFACE
	X11
   )
else()
   message("* Pipeline debug is DISABLED.")
endif()

#
# Unit tests
#
add_test_module(point_tests point.test.cpp)
add_test_module(rect_tests rect.test.cpp)
add_test_module(size_tests size.test.cpp)
add_test_module(histogram_tests histogram.test.cpp)
add_test_module(image_reader_tests io/image_reader.test.cpp)
add_test_module(image_writer_tests io/image_writer.test.cpp)

add_test_module(algorithm_bad_pixel_median_interpolator_tests algorithm/bad_pixel_median_interpolator.test.cpp)
add_test_module(algorithm_average_tests algorithm/average.test.cpp)
add_test_module(algorithm_otsu_thresholder_tests algorithm/threshold/otsu_thresholder.test.cpp)
add_test_module(algorithm_mean_thresholder_tests algorithm/threshold/mean_thresholder.test.cpp)
add_test_module(algorithm_max_entropy_thresholder_tests algorithm/threshold/max_entropy_thresholder.test.cpp)
add_test_module(algorithm_center_of_gravity_centroider_tests algorithm/centroid/center_of_gravity_centroider.test.cpp)
add_test_module(algorithm_intensity_weighted_centroider_tests algorithm/centroid/intensity_weighted_centroider.test.cpp)
add_test_module(algorithm_snr_tests algorithm/snr.test.cpp)
add_test_module(algorithm_hfd_tests algorithm/hfd.test.cpp)
add_test_module(algorithm_fwhm_tests algorithm/fwhm.test.cpp)
add_test_module(algorithm_star_cluster_algorithm_tests algorithm/star_cluster_algorithm.test.cpp)
add_test_module(algorithm_midtone_balance_stretcher_tests algorithm/stretch/midtone_balance_stretcher.test.cpp)

add_test_module(pipeline_views_files_tests views/files.test.cpp)
add_test_module(pipeline_views_read_tests views/read.test.cpp)
add_test_module(pipeline_views_write_tests views/write.test.cpp)
add_test_module(pipeline_views_add_tests views/add.test.cpp)
add_test_module(pipeline_views_subtract_tests views/subtract.test.cpp)
add_test_module(pipeline_views_divide_by_tests views/divide_by.test.cpp)
add_test_module(pipeline_views_multiply_by_tests views/multiply_by.test.cpp)
add_test_module(pipeline_views_detect_stars_tests views/detect_stars.test.cpp)
add_test_module(pipeline_crop_tests views/crop.test.cpp)
add_test_module(pipeline_scale_tests views/scale.test.cpp)
add_test_module(pipeline_interpolate_bad_pixels_tests views/interpolate_bad_pixels.test.cpp)
add_test_module(pipeline_subtract_background_tests views/subtract_background.test.cpp)
add_test_module(pipeline_center_on_star_tests views/center_on_star.test.cpp)
add_test_module(pipeline_view_stretch_tests views/stretch.test.cpp)



# # 
# # Deployment
# # 

# # Deploy generated headers
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)


