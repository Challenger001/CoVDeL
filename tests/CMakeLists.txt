list(APPEND TEST_SOURCE_FILES
  ma/test_dimension.cc
)

list(APPEND TEST_HEADER_FILES
  utils.hh
)

add_executable(test ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test PRIVATE covdel.ma)
target_compile_options(test PRIVATE -Wall)
set_target_properties(test PROPERTIES
  OUTPUT_NAME dimension
  BUILD_WITH_INSTALL_RPATH FALSE
  INSTALL_RPATH "$ORIGIN/../lib"
)

install(TARGETS test RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
