cmake_minimum_required(VERSION 3.15)

##
# test/cmake_usage_test/CMakeLists.txt
#
# Author: Derek Huang
# License: MIT License
#
# newmat11 CMake usage test project.
#
# This project is used to test newmat11 consumption from a CMake project.
#

project(
    newmat11-cmake-test
    DESCRIPTION "newmat11 CMake usage test"
    LANGUAGES CXX
)

################################################################################

# ensure variable is defined/turned on
function(check_var var)
    if(NOT ${var})
        message(FATAL_ERROR "Variable ${var} evaluates to FALSE")
    endif()
endfunction()

# enforce a specific C++ standard on a target
function(std_require target cxx_std)
    set_target_properties(
        ${target} PROPERTIES
        CXX_STANDARD ${cxx_std}
        CXX_STANDARD_REQUIRED ON
    )
endfunction()

################################################################################

include(CTest)

# find newmat11
# TODO: also validate components based on configured build
find_package(newmat11 REQUIRED)
message(STATUS "newmat11 version: ${newmat11_VERSION}")

# report newmat11 features
list(JOIN newmat11_ENABLED_FEATURES " " features_str)
message(STATUS "newmat11 features: ${features_str}")
unset(features_str)

# check if C++11 support is available
if(cxx_std_11 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(have_cxx11 TRUE)
endif()

# TODO: what's the default C++ standard we should use?

# inverse: newmat11 matrix inverse demo program
if(newmat11_namespace_FOUND AND have_cxx11)
    add_executable(inverse inverse.cc)
    std_require(inverse 11)
    target_link_libraries(inverse PRIVATE newmat11::newmat)
    add_test(NAME inverse COMMAND inverse)
else()
    message(STATUS "Skipped inverse (newmat11 namespace + C++11 required)")
endif()
