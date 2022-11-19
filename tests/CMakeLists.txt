# Adds and configures tests for target arguments
function(setup_test TEST_NAME SOURCE_FILE LINK_LIBRARY_LIST)
  add_executable(${TEST_NAME} ${SOURCE_FILE} utils.hh)

  target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(${TEST_NAME} PRIVATE ${LINK_LIBRARY_LIST})
  target_compile_options(${TEST_NAME} PRIVATE -Wall)
  set_target_properties(${TEST_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH FALSE
    INSTALL_RPATH "$ORIGIN/../lib"
  )

  install(TARGETS ${TEST_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endfunction()

setup_test(dimension ma/test_dimension.cc "covdel.ma")
setup_test(multiarray ma/test_multiarray.cc "covdel.ma")
