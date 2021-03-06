cmake_minimum_required(VERSION 3.15)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(WaveGenerator
    VERSION 1.1
    LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
    "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

set(RESOURCE_PATH "${CMAKE_BINARY_DIR}/res")

add_subdirectory(3rd_party/)
add_subdirectory(lib/)
add_subdirectory(app/)
add_subdirectory(res/)