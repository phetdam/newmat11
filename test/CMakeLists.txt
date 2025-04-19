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
        NAME nm11_install_test
        COMMAND ${CMAKE_COMMAND}
                --install ${CMAKE_BINARY_DIR} --prefix ${_test_install_dir}
                --config $<CONFIG>
    )
else()
    add_test(
        NAME nm11_install_test
        COMMAND ${CMAKE_COMMAND}
                --install ${CMAKE_BINARY_DIR} --prefix ${_test_install_dir}
    )
endif()
# set up test_install test fixture
set_tests_properties(
    nm11_install_test PROPERTIES
    FIXTURES_SETUP test_install
    FIXTURES_REQUIRED test_install_clean
)
