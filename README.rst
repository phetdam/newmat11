.. README.rst

newmat11
========

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
of which a copy has been provided in the source tree for offline viewing.

The CMake_ build and packaging support was contributed by Derek Huang to
facilitate a standard build mechanism across POSIX-like and Windows platforms.
All other files are from the `newmat11.zip`_ downloaded on April 13, 2025.
Originally, the library only targeted C++98, so some modifications have been
made by Derek Huang to enable compilation under C++11 or above.

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

Once located, one can link against the library using the provided target:

.. code:: cmake

   target_link_libraries(my_program PRIVATE newmat11::newmat)
