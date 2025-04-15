cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

##
# newmat11/CMakeLists.txt
#
# Author: Derek Huang
# Copyright: MIT License
#

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
    # to support namespaced header inclusion from the build tree
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    # to support original non-namespaced header inclusion
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

# build newmat11 examples
# example: example program
newmat11_add_example(example)
# TODO: fix causes of C4302 and C4311
if(MSVC)
    target_compile_options(example PRIVATE /wd4302 /wd4311)
endif()
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
# TODO: fix causes of MSVC warnings
if(MSVC)
    target_compile_options(test_exc PRIVATE /wd4302 /wd4311 /wd4456)
endif()

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
    # TODO: fix causes of MSVC warnings
    if(MSVC)
        target_compile_options(tmt PRIVATE /wd4302 /wd4311 /wd4456)
    endif()
    add_test(NAME tmt COMMAND tmt)
else()
    message(STATUS "Skipping ${PROJECT_NAME} test target tmt")
endif()
