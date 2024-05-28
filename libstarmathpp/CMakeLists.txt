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
)


# 
# Build library
#
add_library(${target} ${sources})

message("* TIFF LIBS : ${TIFF_LIBRARIES}")


# Other libraries the starmathpp library depends on
target_link_libraries(${target}
	INTERFACE
	${TIFF_LIBRARIES}
	PRIVATE
	${CFITSIO_LIBRARY}
	${CCFITS_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
)


#
# Unit tests
#
add_test_module(point_tests point.test.cpp)
add_test_module(rect_tests rect.test.cpp)
add_test_module(size_tests size.test.cpp)
add_test_module(histogram_tests histogram.test.cpp)
add_test_module(image_reader_tests io/image_reader.test.cpp)
add_test_module(image_writer_tests io/image_writer.test.cpp)
add_test_module(pipeline_views_files_tests pipeline/views/files.test.cpp)
add_test_module(pipeline_views_read_tests pipeline/views/read.test.cpp)
add_test_module(pipeline_views_write_tests pipeline/views/write.test.cpp)
add_test_module(pipeline_views_add_tests pipeline/views/add.test.cpp)
add_test_module(pipeline_views_subtract_tests pipeline/views/subtract.test.cpp)
add_test_module(pipeline_views_divide_by_tests pipeline/views/divide_by.test.cpp)
add_test_module(pipeline_views_multiply_by_tests pipeline/views/multiply_by.test.cpp)
add_test_module(pipeline_crop_tests pipeline/views/crop.test.cpp)
add_test_module(pipeline_scale_tests pipeline/views/scale.test.cpp)
add_test_module(pipeline_interpolate_bad_pixels_tests pipeline/views/interpolate_bad_pixels.test.cpp)
add_test_module(pipeline_subtract_background_tests pipeline/views/subtract_background.test.cpp)
add_test_module(algorithm_bad_pixel_median_interpolator_tests algorithm/bad_pixel_median_interpolator.test.cpp)
add_test_module(algorithm_average_tests algorithm/average.test.cpp)
add_test_module(algorithm_otsu_thresholder_tests algorithm/threshold/otsu_thresholder.test.cpp)
add_test_module(algorithm_mean_thresholder_tests algorithm/threshold/mean_thresholder.test.cpp)
add_test_module(algorithm_max_entropy_thresholder_tests algorithm/threshold/max_entropy_thresholder.test.cpp)




# # 
# # Deployment
# # 

# # Deploy generated headers
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)





#
# TODO: Remove list of lib names when no longer needed...
#
#
# ${CFITSIO_LIBRARY}
# ${CCFITS_LIBRARY}
# ${GSL_LIBRARY}
# ${GSL_CBLAS_LIBRARY}
# ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
# ${Boost_LOG_LIBRARY}
# ${Boost_SYSTEM_LIBRARY}
# ${DEFAULT_LINKER_OPTIONS}
# ${TIFF_LIBRARIES}
# X11


