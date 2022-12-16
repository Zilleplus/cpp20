add_executable(rgen_tests generator_tests.cpp)
target_link_libraries(rgen_tests PUBLIC rgen catch2::catch2)
