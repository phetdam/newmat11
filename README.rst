.. README.rst

newmat11
========

   Note:

   All source and other newmat files will be moved into a ``newmat11/``
   directory to keep the top-level directory cleaner. This organizational
   change will be documented in later updates to this file.

A C++ matrix library by Robert B. Davies.

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
HTML file ``nm11.htm`` in the directory root).

The CMake_ build and packaging support was contributed by Derek Huang to
facilitate a standard build mechanism across POSIX-like and Windows platforms.
All other files are from the `newmat11.zip`_ downloaded on April 13, 2025.
Originally, the library only targeted C++98, so some modifications have been
made by Derek Huang to enable compilation under C++11 or above and to reduce
the amount of warnings emitted during compilation.

.. __: https://www.robertnz.net/nm_intro.htm
.. __: https://www.robertnz.net/nm11.htm
.. _newmat11.zip: https://www.robertnz.net/ftp/newmat11.zip
.. _CMake: https://cmake.org/cmake/help/latest/


Building
--------

To build, CMake 3.21 or above is required, as well as a C++98 compiler. By
default, CMake will build newmat11 with the "default" C++ standard supported by
the compiler, but this can be changed via different CMake settings.

TBD


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
     auto Y = X.i();
     std::cout << "Inverse of X\n" << fmt << Y << std::endl;
     // multiply X by Y and print the result (should be near identity)
     std::cout << "X * inverse of X\n" << fmt << (X * Y) << std::endl;
     return EXIT_SUCCESS;
   }

We can compile and link the program against newmat11 in CMake as follows:

.. code:: cmake

   add_executable(inverse inverse.cc)
   target_link_libraries(inverse PRIVATE newmat11::newmat)
