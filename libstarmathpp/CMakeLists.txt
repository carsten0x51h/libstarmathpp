message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_SOURCE_DIR}")


#
# Target name
#
set(target starmathpp)


#
# Generate version-header
#
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)


# 
# Sources
#
set(sources
	point.cpp
)


# 
# Build library
#
add_library(${target} ${sources})


#
# Unit tests
#
add_executable(point_test point.test.cpp)
target_link_libraries(point_test
	PRIVATE
	${target}
#	${DEFAULT_LIBRARIES}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#	${Boost_LOG_LIBRARY}
#	${Boost_SYSTEM_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
#	${CMAKE_SOURCE_DIR}/${target}
)

# now register the executable with CTest
add_test(NAME point_test COMMAND point_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


# target_link_libraries(point_test
# 		PRIVATE
#         ${DEFAULT_LIBRARIES}
# 		${CFITSIO_LIBRARY}
# 		${CCFITS_LIBRARY}
# 		${GSL_LIBRARY}
# 		${GSL_CBLAS_LIBRARY}
# 		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#         ${Boost_LOG_LIBRARY}
#         ${Boost_SYSTEM_LIBRARY}
#         ${DEFAULT_LINKER_OPTIONS}
# 		${TIFF_LIBRARIES}
# 		X11
# 		)





#add_test(NAME mytest COMMAND testDriver --config $<CONFIG> --exe $<TARGET_FILE:myexe>)

# # 
# # Sub-projects
# #
# set(IDE_FOLDER "")
# add_subdirectory(focus_finder)


# # 
# # Deployment
# # 

# # Deploy generated headers
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
