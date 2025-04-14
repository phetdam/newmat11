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
option(NEWMAT11_ENABLE_C_SUBSCRIPTS "Enable C-style bracketed subscripting" OFF)
option(NEWMAT11_BUILD_EXAMPLES "Build the newmat11 example programs" OFF)
option(NEWMAT11_BUILD_TESTS "Build the newmat11 test program" OFF)

include(CTest)

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

# info on options
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
    add_compile_options(/W4)
elseif(NEWMAT11_GNU_COMPILER)
    add_compile_options(-Wall)
endif()

# newmat: the newmat C++ matrix library
add_library(
    newmat STATIC
    bandmat.cpp
    cholesky.cpp
    evalue.cpp
    fft.cpp
    hholder.cpp
    jacobi.cpp
    myexcept.cpp
    newfft.cpp
    newmat1.cpp
    newmat2.cpp
    newmat3.cpp
    newmat4.cpp
    newmat5.cpp
    newmat6.cpp
    newmat7.cpp
    newmat8.cpp
    newmat9.cpp
    newmatex.cpp
    newmatnl.cpp
    newmatrm.cpp
    nm_misc.cpp
    solution.cpp
    sort.cpp
    submat.cpp
    svd.cpp
)
# alias for in-tree usage
add_library(${PROJECT_NAME}::newmat ALIAS newmat)
target_include_directories(
    newmat PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
# in several files GCC will emit -Wmisleading-indentation
if(NEWMAT11_GNU_COMPILER)
    target_compile_options(newmat PRIVATE -Wno-misleading-indentation)
endif()
# custom configuration macros
if(NEWMAT11_USE_NAMESPACE)
    target_compile_definitions(newmat PUBLIC use_namespace)
endif()
if(NEWMAT11_ENABLE_FREE_CHECK)
    target_compile_definitions(newmat PUBLIC DO_FREE_CHECK)
endif()
if(NEWMAT11_ENABLE_C_SUBSCRIPTS)
    target_compile_definitions(newmat PUBLIC SETUP_C_SUBSCRIPTS)
endif()

# newmat_nl: newmat nonlinear optimization routines
add_library(newmat_nl STATIC newmatnl.cpp)
add_library(${PROJECT_NAME}::newmat_nl ALIAS newmat_nl)
target_link_libraries(newmat_nl PUBLIC newmat)

# newmat_solve: newmat one-dimensional solution routines
add_library(newmat_solve STATIC solution.cpp)
add_library(${PROJECT_NAME}::newmat_solve ALIAS newmat_solve)
target_link_libraries(newmat_solve PUBLIC newmat)

# ensure all libraries have the 'd' debug suffix
set_target_properties(
    newmat
    newmat_nl
    newmat_solve PROPERTIES
    DEBUG_POSTFIX d
)

# CMake script installation directory
set(NEWMAT11_CMAKE_PREFIX lib/cmake/${PROJECT_NAME})

# library target install rules
install(TARGETS newmat newmat_nl newmat_solve EXPORT ${PROJECT_NAME}-targets)
# install target export file
install(
    EXPORT ${PROJECT_NAME}-targets
    DESTINATION ${NEWMAT11_CMAKE_PREFIX}
    NAMESPACE ${PROJECT_NAME}::
)
# public header file install rule
install(
    FILES
        controlw.h
        include.h
        myexcept.h
        newmat.h
        newmatap.h
        newmatio.h
        newmatnl.h
        newmatrc.h
        newmatrm.h
        precisio.h
        solution.h
    # note: namespacing directory added to prevent conflicts
    DESTINATION include/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

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

##
# Helper function for registering a newmat example program.
#
# This builds a single ${target}.cpp file into an example program, links
# against the newmat library target, and registers a CTest test However, if
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
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

# build examples
# example: example program
newmat11_add_example(example)
# garch: GARCH model fitting using maximum likelihood
newmat11_add_example(garch LIBRARIES newmat_nl)
# nm_ex1: matrix inversion
newmat11_add_example(nm_ex1)
# nm_ex2: Hilbert matrix eigenvalues
newmat11_add_example(nm_ex2)
# nm_ex3: precisio.h scientific format + constants
newmat11_add_example(nm_ex3)
# nl_ex: nonlinear least squares example
newmat11_add_example(nl_ex LIBRARIES newmat_nl)
# sl_ex: OneDimSolve example
newmat11_add_example(sl_ex LIBRARIES newmat_solve)
# test_exc: exception handling demo
newmat11_add_example(test_exc)

# build test program
if(NEWMAT11_BUILD_TESTS)
    # tmt: newmat11 test program
    add_executable(
        tmt
        tmt.cpp
        tmt1.cpp
        tmt2.cpp
        tmt3.cpp
        tmt4.cpp
        tmt5.cpp
        tmt6.cpp
        tmt7.cpp
        tmt8.cpp
        tmt9.cpp
        tmta.cpp
        tmtb.cpp
        tmtc.cpp
        tmtd.cpp
        tmte.cpp
        tmtf.cpp
        tmtg.cpp
        tmth.cpp
        tmti.cpp
        tmtj.cpp
        tmtk.cpp
        tmtl.cpp
        tmtm.cpp
    )
    target_link_libraries(tmt PRIVATE newmat)
    # for tmt.cpp GCC will emit -Wmisleading-indentation
    if(NEWMAT11_GNU_COMPILER)
        target_compile_options(tmt PRIVATE -Wno-misleading-indentation)
    endif()
    add_test(NAME tmt COMMAND tmt)
else()
    message(STATUS "Skipping ${PROJECT_NAME} test target tmt")
endif()
