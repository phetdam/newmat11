cmake_minimum_required(VERSION 3.15)

##
# CMakeLists.txt
#
# Author: Derek Huang
# License: MIT License
#
# CMake build and installation configuration for newmat11.
#

project(
    newmat11
    VERSION 0.1.0
    DESCRIPTION "A C++ matrix library by Robert B. Davies"
    HOMEPAGE_URL https://www.robertnz.net/nm_intro.htm
    LANGUAGES CXX
)

# build-time options
option(NEWMAT11_USE_NAMESPACE "Ensure all symbols are namespaced" ON)
option(NEWMAT11_ENABLE_FREE_CHECK "Enable FREE_CHECK allocation tracking" OFF)
option(NEWMAT11_ENABLE_C_SUBSCRIPTS "Enable C-style bracketed subscripting" ON)
option(NEWMAT11_BUILD_EXAMPLES "Build the newmat11 example programs" OFF)
option(NEWMAT11_BUILD_TESTS "Build the newmat11 test program" OFF)

# update CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(CTest)
include(newmat11_add_example)
include(newmat11_collect_build_options)

# ensure BUILD_TESTING and NEWMAT11_BUILD_TESTS are aligned
set(BUILD_TESTING ${NEWMAT11_BUILD_TESTS})

# ensure flat binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# note: no C++ standard automatically set. if you want to compile under a
# specific C++ standard please use -DCMAKE_CXX_STANDARD

# multi-config generator determination + build type
get_property(NEWMAT11_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NEWMAT11_IS_MULTI_CONFIG)
    message(STATUS "Build config: Multi")
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
    endif()
    message(STATUS "Build config: ${CMAKE_BUILD_TYPE}")
endif()

# info on build features
if(NEWMAT11_USE_NAMESPACE)
    message(STATUS "Enable namespacing: Yes")
else()
    message(STATUS "Enable namespacing: No")
endif()
if(NEWMAT11_ENABLE_FREE_CHECK)
    message(STATUS "Enable FREE_CHECK: Yes")
else()
    message(STATUS "Enable FREE_CHECK: No")
endif()
if(NEWMAT11_ENABLE_C_SUBSCRIPTS)
    message(STATUS "Enable C-style indexing: Yes")
else()
    message(STATUS "Enable C-style indexing: No")
endif()

# collect enabled build options into feature list
newmat11_collect_build_options(
    NEWMAT11_ENABLED_FEATURES
    OPTIONS
        NEWMAT11_USE_NAMESPACE
        NEWMAT11_ENABLE_FREE_CHECK
        NEWMAT11_ENABLE_C_SUBSCRIPTS
)
# print enabled features
list(JOIN NEWMAT11_ENABLED_FEATURES " " features_str)
message(STATUS "Build features: ${features_str}")
unset(features_str)

# info on build config
if(NEWMAT11_BUILD_EXAMPLES)
    message(STATUS "Build examples: Yes")
else()
    message(STATUS "Build examples: No")
endif()
if(NEWMAT11_BUILD_TESTS)
    message(STATUS "Build tests: Yes")
else()
    message(STATUS "Build tests: No")
endif()

# indicate if compiler is GNU-like
if(
    CMAKE_CXX_COMPILER_ID STREQUAL AppleClang OR
    CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
    CMAKE_CXX_COMPILER_ID STREQUAL GNU
)
    set(NEWMAT11_GNU_COMPILER TRUE)
else()
    set(NEWMAT11_GNU_COMPILER FALSE)
endif()

# compiler options
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
elseif(NEWMAT11_GNU_COMPILER)
    add_compile_options(-Wall)
endif()

# CMake script installation directory
set(NEWMAT11_CMAKE_PREFIX lib/cmake/${PROJECT_NAME})

# configure newmat libraries, example programs, and tmt test program
add_subdirectory(newmat11)
# configure tests
add_subdirectory(test)

# configure CMake config + version config scripts
configure_package_config_file(
    cmake/${PROJECT_NAME}-config.cmake.in
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${NEWMAT11_CMAKE_PREFIX}
)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY AnyNewerVersion
)
# install rule for the config scripts
install(
    FILES
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${NEWMAT11_CMAKE_PREFIX}
)
