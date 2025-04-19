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
  int width_;      // field width
  int precision_;  // precision
};

std::ostream& operator<<(std::ostream& out, const float_format& ff)
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
