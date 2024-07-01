macro(add_test_module TEST_MODULE_NAME TEST_SOURCES)
   message(STATUS "Adding test module ${TEST_MODULE_NAME}...")
   add_executable(${TEST_MODULE_NAME} ${TEST_SOURCES})
   target_link_libraries(${TEST_MODULE_NAME}
	PRIVATE
	${target}
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${DEFAULT_LINKER_OPTIONS}
   )
   add_test(NAME ${TEST_MODULE_NAME} COMMAND ${TEST_MODULE_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

endmacro()
