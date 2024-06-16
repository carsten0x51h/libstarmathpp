include (AddUnitTest)

#
# Target name
#
set(target starmathpp)


#
# Integration tests
#
add_test_module(image_development_tests image_development.test.cpp)
add_test_module(star_metrics_integration_tests star_metrics.test.cpp)
add_test_module(star_recognizer_integration_tests star_recognizer.test.cpp)


# # 
# # Deployment
# # 

# # Deploy generated headers
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
