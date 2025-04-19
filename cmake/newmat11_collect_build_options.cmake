cmake_minimum_required(VERSION 3.15)

##
# newmat11_collect_build_options.cmake
#
# Provides a function to collect the build customization options in a list.
#

##
# Collect the given build customization options into a list of features.
#
# Each OPTIONS argument set to TRUE received by the function will be converted
# to a build option name using the following simple formula:
#
#   1. Convert the variable name to lowercase
#   2. Remove newmat11_use_ or newmat11_enable_ prefix
#
# These feature names can be specified with COMPONENTS in find_package.
#
# Arguments:
#   options             List variable to write feature names into
#   OPTIONS var...      Variables taking TRUE/FALSE values to convert
#
function(newmat11_collect_build_options options)
    cmake_parse_arguments(ARG "" "" "OPTIONS" ${ARGN})
    # must have at least one option
    if(NOT ARG_OPTIONS)
        message(FATAL_ERROR "OPTIONS must have at least one argument")
    endif()
    # collect options set to TRUE
    foreach(_opt ${ARG_OPTIONS})
        # add to list of enabled build options if active
        if(${${_opt}})
            string(TOLOWER "${_opt}" _lower_opt)
            string(REPLACE "${PROJECT_NAME}_use_" "" _lower_opt "${_lower_opt}")
            string(REPLACE "${PROJECT_NAME}_enable_" "" _lower_opt "${_lower_opt}")
            list(APPEND build_options "${_lower_opt}")
        endif()
    endforeach()
    # set options
    set(${options} ${build_options} PARENT_SCOPE)
endfunction()