cmake_minimum_required(VERSION 3.23)
project(cpp20)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
	add_compile_options(/W4)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES catch2/3.2.0
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(Catch2)

add_subdirectory(coroutines)