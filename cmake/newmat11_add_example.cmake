cmake_minimum_required(VERSION 3.15)

##
# newmat11_add_example.cmake
#
# Author: Derek Huang
# License: MIT License
#
# Provides a function to simplify adding a single-source newmat11 example.
#

##
# Helper function for registering a newmat example program.
#
# This builds a single ${target}.cpp file into an example program, links
# against the newmat library target, and registers a CTest test. However, if
# NEWMAT11_BUILD_EXAMPLES is FALSE, a skip message is printed instead.
#
# The working directory of each test is the top-level source directory.
#
# Arguments:
#   target                  Program target name
#   [LIBRARIES libs...]     Additional libraries to link against
#
function(newmat11_add_example target)
    # not building examples
    if(NOT NEWMAT11_BUILD_EXAMPLES)
        message(STATUS "Skipping ${PROJECT_NAME} example ${target}")
        return()
    endif()
    # otherwise, parse libraries and add program
    cmake_parse_arguments(ARG "" "" "LIBRARIES" ${ARGN})
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PRIVATE newmat)
    if(ARG_LIBRARIES)
        target_link_libraries(${target} PRIVATE ${ARG_LIBRARIES})
    endif()
    # register CTest test
    add_test(
        NAME ${target}
        COMMAND ${target}
        # ensures that any data files in the same directory as the program
        # source can be read without any changes to working directory
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()
