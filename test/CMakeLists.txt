cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

##
# test/CMakeLists.txt
#
# Author: Derek Huang
# License: MIT License
#

# if tests disabled, stop processing
if(NOT NEWMAT11_BUILD_TESTS)
    message(STATUS "Skipping CMake install test")
    return()
endif()

# directory to test install directory
set(_test_install_dir ${CMAKE_BINARY_DIR}/nm11_test_install)

# ensure clean test install
add_test(
    NAME nm11_install_prepare
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${_test_install_dir}
)
# set up test_install_clean test fixture
set_tests_properties(
    nm11_install_prepare PROPERTIES
    FIXTURES_SETUP test_install_clean
)

# install test
if(NEWMAT11_IS_MULTI_CONFIG)
    add_test(
        NAME nm11_install
        COMMAND ${CMAKE_COMMAND}
                --install ${CMAKE_BINARY_DIR} --prefix ${_test_install_dir}
                --config $<CONFIG>
    )
else()
    add_test(
        NAME nm11_install
        COMMAND ${CMAKE_COMMAND}
                --install ${CMAKE_BINARY_DIR} --prefix ${_test_install_dir}
    )
endif()
# set up test_install test fixture
set_tests_properties(
    nm11_install PROPERTIES
    FIXTURES_SETUP test_install
    FIXTURES_REQUIRED test_install_clean
)

# source directory for CMake usage test
set(_cmake_usage_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake_usage_test)
# build directory for CMake usage test
set(_cmake_usage_build_dir ${CMAKE_BINARY_DIR}/nm11_cmake_test)

# ensure clean CMake usage test
add_test(
    NAME nm11_cmake_prepare
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${_cmake_usage_build_dir}
)
# set up cmake_clean test fixture
# note: can run in parallel with install setup
set_tests_properties(
    nm11_cmake_prepare PROPERTIES
    FIXTURES_SETUP cmake_clean
)

# CMake configure test
add_test(
    NAME nm11_cmake_config
    COMMAND ${CMAKE_COMMAND}
            -S ${_cmake_usage_src_dir} -B ${_cmake_usage_build_dir}
            -D${PROJECT_NAME}_ROOT=${_test_install_dir}
)
# set up cmake_config test fixture
set_tests_properties(
    nm11_cmake_config PROPERTIES
    FIXTURES_SETUP cmake_config
    FIXTURES_REQUIRED "test_install;cmake_clean"
)

# CMake build test
if(NEWMAT11_IS_MULTI_CONFIG)
    add_test(
        NAME nm11_cmake_build
        COMMAND ${CMAKE_COMMAND}
                --build ${_cmake_usage_build_dir} --config $<CONFIG>
    )
else()
    add_test(
        NAME nm11_cmake_build
        COMMAND ${CMAKE_COMMAND} --build ${_cmake_usage_build_dir}
    )
endif()
# set up cmake_build test fixture
set_tests_properties(
    nm11_cmake_build PROPERTIES
    FIXTURES_SETUP cmake_build
    FIXTURES_REQUIRED cmake_config
)

# run CTest tests
# note: can use --test-dir for CMake >= 3.20
if(NEWMAT11_IS_MULTI_CONFIG)
    add_test(
        NAME nm11_cmake_ctest
        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
        WORKING_DIRECTORY ${_cmake_usage_build_dir}
    )
else()
    add_test(
        NAME nm11_cmake_ctest
        COMMAND ${CMAKE_CTEST_COMMAND}
        WORKING_DIRECTORY ${_cmake_usage_build_dir}
    )
endif()
# set up cmake_ctest test fixture
set_tests_properties(
    nm11_cmake_ctest PROPERTIES
    FIXTURES_SETUP cmake_ctest
    FIXTURES_REQUIRED cmake_build
)
