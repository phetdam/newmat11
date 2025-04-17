.. README.rst

newmat11
========

A C++ matrix library by `Robert B. Davies`__.

.. __: https://www.robertnz.net/

Supports different types of matrices including:

 * Upper triangular
 * Lower triangular
 * Symmetric
 * Banded
 * Row/column vectors

Provides useful matrix operations such as:

 * Addition
 * Multiplication
 * Inverse
 * Kronecker product
 * Hadamard/Schur product
 * Transpose
 * Cholesky decomposition
 * QR factorization
 * SVD

See the full features in the `online summary`__ and the `online documentation`__,
of which a copy has been provided in the source tree for offline viewing (the
HTML file ``/newmat11/nm11.htm``).

The CMake_ build and packaging support was contributed by Derek Huang to
facilitate a standard build mechanism across POSIX-like and Windows platforms.
All other files in the ``/newmat11`` directory are from the `newmat11.zip`_
downloaded on April 13, 2025. Originally, the library only targeted C++98, so
some modifications have been made by Derek Huang to enable compilation under
C++11 or above and to reduce compiler warnings.

.. __: https://www.robertnz.net/nm_intro.htm
.. __: https://www.robertnz.net/nm11.htm
.. _newmat11.zip: https://www.robertnz.net/ftp/newmat11.zip
.. _CMake: https://cmake.org/cmake/help/latest/


Contents
--------

To clearly indicate a separation between Davies' sources and additional CMake
and example programs, the source tree is laid out as follows:

cmake
   CMake modules

example
   C++11 example programs

newmat11
   ``newmat11.zip`` sources updated to compile under C++11


Installation
------------

To build, CMake 3.20 or above is required, as well as a C++98 compiler. By
default, CMake will build newmat11 with the "default" C++ standard supported by
the compiler, but this can be changed via CMAKE_CXX_STANDARD_. There are also
a few CMake options that can be passed using ``-D<var>=<value>`` to configure
the build:

+----------------------------+---------+--------------------------------------+
| Option                     | Default | Description                          |
+============================+=========+======================================+
| ``NEWMAT11_USE_NAMESPACE`` | ``ON``  | Always enclose symbols in namespace. |
|                            |         | This ensures that newmat11 symbols   |
|                            |         | are accessed from the ``NEWMAT::``   |
|                            |         | namespace.                           |
+----------------------------+---------+--------------------------------------+

.. _CMAKE_CXX_STANDARD:
   https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html

Since there are some differences between how CMake is traditionally invoked on
POSIX-like systems vs. on Windows, we have two separate subsections with
instructions on building, running tests, and installing newmat11 artifacts.

\*nix
~~~~~

To configure a release build, use the following command:

.. code::

   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

Then, to build the newmat11 library files, use the following command:

.. code::

   cmake --build build -j

This will invoke your native platform build tool, e.g. Make. If examples and
tests were enabled by passing to CMake the ``-DNEWMAT11_BUILD_EXAMPLES=ON``
and/or ``-DNEWMAT11_BUILD_TESTS=ON`` options, examples and tests can be run with:

.. code::

   ctest --test-dir build -j$(nproc)

To install the newmat11 libraries into e.g. ``/opt/newmat11``, use:

.. code::

   cmake --install build --prefix /opt/newmat11

Windows
~~~~~~~

To configure a 64-bit build using the latest Visual Studio generator, run the
following in a `Developer Command Prompt`_ shell or in `Windows Terminal`_ tab
running the Developer Command Prompt profile:

.. _Developer Command Prompt:
   https://learn.microsoft.com/en-us/visualstudio/ide/reference/
   command-prompt-powershell?view=vs-2022

.. _Windows Terminal: https://learn.microsoft.com/en-us/windows/terminal/

.. code::

   cmake -S . -B build -A x64

To build the newmat11 Release configuration, use the following:

.. code::

   cmake --build build --config Release -j


If examples and tests were enabled, run the Release examples and tests with:

.. code::

   ctest --test-dir build -C Release -j%NUMBER_OF_PROCESSORS%

Finally, the install the newmat11 release libraries into a install prefix, use:

.. code::

   cmake --install build --prefix <install-prefix> --config Release

Note that if you would also like to have the Debug configuration built, which
by default will link against the dynamic debug `C runtime`__, repeat the above
steps for building, testing, and installing, but with Release replaced with Debug.

.. __: https://learn.microsoft.com/en-us/cpp/c-runtime-library/
       crt-library-features?view=msvc-170


Using newmat11
--------------

After having installed newmat11 one can use it from CMake with `find_package`_:

.. _find_package: https://cmake.org/cmake/help/latest/command/find_package.html

.. code:: cmake

   find_package(newmat11 REQUIRED)

Suppose we have the following C++11 program demonstrating matrix inversion:

.. code:: cpp

   /**
    * @file inverse.cc
    * @author Derek Huang
    * @brief C++ newmat11 matrix inverse example
    * @copyright MIT License
    *
    * @file This is a C++11 version of nm_ex1.cpp that works with CMake installs.
    */

   #include <cstdlib>
   #include <iomanip>
   #include <iostream>

   #include <newmat11/newmat.h>
   #include <newmat11/newmatio.h>  // for matrix operator<<

   namespace {

   // float formatting object
   struct float_format {
     int width_;
     int precision_;
   };

   auto& operator<<(std::ostream& out, const float_format& ff)
   {
     return out << std::setw(ff.width_) << std::setprecision(ff.precision_);
   }

   }  // namespace

   int main()
   {
     // stream formatter
     constexpr float_format fmt{15, 8};
     // create matrix row by row
     NEWMAT::Matrix X(4, 4);
     X.row(1) <<  3.7 << -2.1 <<  7.4 << -1.0;
     X.row(2) <<  4.1 <<  0.0 <<  3.9 <<  4.0;
     X.row(3) << -2.5 <<  1.9 << -0.4 <<  7.3;
     X.row(4) <<  1.5 <<  9.8 << -2.1 <<  1.1;
     // print the matrix X
     std::cout << "Matrix X\n" << fmt << X << std::endl;
     // print matrix inverse Y
     // note: i() returns an InvertedMatrix that owns no memory, not a Matrix
     NEWMAT::Matrix Y = X.i();
     std::cout << "Inverse of X\n" << fmt << Y << std::endl;
     // multiply X by Y and print the result (should be near identity)
     std::cout << "X * inverse of X\n" << fmt << (X * Y) << std::endl;
     return EXIT_SUCCESS;
   }

We can compile and link the program against newmat11 in CMake as follows:

.. code:: cmake

   add_executable(inverse inverse.cc)
   target_link_libraries(inverse PRIVATE newmat11::newmat)
