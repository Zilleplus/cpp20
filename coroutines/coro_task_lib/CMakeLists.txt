add_library(tasklib STATIC task.cpp)
target_include_directories(tasklib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(tasklib_tests ${CMAKE_CURRENT_LIST_DIR}/tests/task_tests.cpp )

target_link_libraries(tasklib_tests catch2::catch2)
target_link_libraries(tasklib_tests tasklib)
