message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_SOURCE_DIR}")


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
)


# 
# Build library
#
add_library(${target} ${sources})


#
# Unit tests
#

# Point tests
add_executable(point_tests point.test.cpp)
target_link_libraries(point_tests
	PRIVATE
	${target}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
)
add_test(NAME point_tests COMMAND point_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


# Rect tests
add_executable(rect_tests rect.test.cpp)
target_link_libraries(rect_tests
	PRIVATE
	${target}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
)
add_test(NAME rect_tests COMMAND rect_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})



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


