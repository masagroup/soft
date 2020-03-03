CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# vcpkg cmake toolchain file 
if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE STRING "vcpk root directory")
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(library CXX)

enable_testing()

get_filename_component(CMAKE_DEPS_PATH "${CMAKE_INSTALL_PREFIX}/cmake" ABSOLUTE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DEPS_PATH})

#ecore
add_subdirectory(lib)
add_subdirectory(tests)
