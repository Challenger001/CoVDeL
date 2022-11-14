list(APPEND MA_SOURCE_FILES
  multiarray.cc
)

list(APPEND MA_HEADER_FILES
)

add_library(covdel.ma SHARED ${MA_SOURCE_FILES} ${MA_HEADER_FILES})

target_include_directories(covdel.ma PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(covdel.ma PUBLIC -Wall)

install(TARGETS covdel.ma LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib)
