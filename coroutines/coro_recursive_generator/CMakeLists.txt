add_library(rgen generator.cpp)
target_include_directories(rgen PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(tests)