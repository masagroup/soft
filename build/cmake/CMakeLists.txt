CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# vcpkg cmake toolchain file 
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# soft root dir
get_filename_component(SOFT_ROOT_DIR ${CMAKE_SOURCE_DIR}/../.. ABSOLUTE)

# update cmake search path for packages/modules
list(APPEND CMAKE_MODULE_PATH ${SOFT_ROOT_DIR}/build/cmake/modules)

#update cmake install path
set( CMAKE_INSTALL_PREFIX "${SOFT_ROOT_DIR}")

project(soft CXX)

enable_testing()

#ecore
add_subdirectory(${SOFT_ROOT_DIR}/libraries/ecore ${CMAKE_CURRENT_BINARY_DIR}/ecore)
add_subdirectory(${SOFT_ROOT_DIR}/tests/ecore.tests ${CMAKE_CURRENT_BINARY_DIR}/ecore.tests)