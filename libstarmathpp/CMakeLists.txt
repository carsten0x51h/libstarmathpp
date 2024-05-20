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

# Other libraries the starmathpp library depends on
target_link_libraries(${target}
	PRIVATE
	${CFITSIO_LIBRARY}
	${CCFITS_LIBRARY}
	${TIFF_LIBRARIES}
	${DEFAULT_LINKER_OPTIONS}
)

#
# Unit tests
#
add_test_module(point_tests point.test.cpp)
add_test_module(rect_tests rect.test.cpp)
add_test_module(image_reader_tests io/image_reader.test.cpp)
add_test_module(pipeline_views_read_tests pipeline/views/read.test.cpp)
add_test_module(image_writer_tests io/image_writer.test.cpp)
add_test_module(pipeline_views_write_tests pipeline/views/write.test.cpp)
add_test_module(pipeline_views_add_tests pipeline/views/add.test.cpp)
add_test_module(pipeline_views_subtract_tests pipeline/views/subtract.test.cpp)
add_test_module(pipeline_views_divide_by_tests pipeline/views/divide_by.test.cpp)
add_test_module(pipeline_views_multiply_by_tests pipeline/views/multiply_by.test.cpp)
add_test_module(bad_pixel_median_interpolator_tests algorithm/bad_pixel_median_interpolator.test.cpp)



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


