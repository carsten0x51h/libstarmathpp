include (AddUnitTest)

#
# Target name
#
set(target starmathpp)

#
# Integration tests
#
add_test_module(image_development_tests image_development_test.cpp)


# # 
# # Deployment
# # 

# # Deploy generated headers
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
