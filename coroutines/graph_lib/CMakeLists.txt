add_library(graphlib STATIC generator.cpp graph.cpp)
target_include_directories(graphlib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(graphlib_tests 
	${CMAKE_CURRENT_LIST_DIR}/tests/graph_lib_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/generator_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/graph_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/algorithms/breathfirst_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/algorithms/depthfirst_tests.cpp
	)

target_link_libraries(graphlib_tests catch2::catch2)
target_link_libraries(graphlib_tests graphlib)