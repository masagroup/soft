CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# vcpkg cmake toolchain file 
if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE STRING "vcpk root directory")
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# soft root dir
get_filename_component(CPP_ROOT_DIR ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)

# update cmake search path for packages/modules
list(APPEND CMAKE_MODULE_PATH ${CPP_ROOT_DIR}/cmake)

project(library CXX)

enable_testing()

#ecore
add_subdirectory(lib)
add_subdirectory(tests)
