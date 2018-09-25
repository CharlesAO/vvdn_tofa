// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if defined EIGEN_TEST_PART_1 || defined EIGEN_TEST_PART_2 || defined EIGEN_TEST_PART_3 || defined EIGEN_TEST_PART_4
#define EIGEN_DONT_ALIGN
#elif defined EIGEN_TEST_PART_5 || defined EIGEN_TEST_PART_6 || defined EIGEN_TEST_PART_7 || defined EIGEN_TEST_PART_8
#define EIGEN_DONT_ALIGN_STATICALLY
#endif

#include "main.h"
#include <Eigen/Dense>

#ifdef CYCLESCOUNT
#include "CyclesCount.h"
#endif

template<typename MatrixType>
void dontalign(const MatrixType& m)
{
  typedef typename MatrixType::Index Index;
  typedef typename MatrixType::Scalar Scalar;
  typedef Matrix<Scalar, MatrixType::RowsAtCompileTime, 1> VectorType;
  typedef Matrix<Scalar, MatrixType::RowsAtCompileTime, MatrixType::RowsAtCompileTime> SquareMatrixType;

  Index rows = m.rows();
  Index cols = m.cols();

  MatrixType a = MatrixType::Random(rows,cols);
  SquareMatrixType square = SquareMatrixType::Random(rows,rows);
  VectorType v = VectorType::Random(rows);

  VERIFY_IS_APPROX(v, square * square.colPivHouseholderQr().solve(v));
  square = square.inverse().eval();
  a = square * a;
  square = square*square;
  v = square * v;
  v = a.adjoint() * v;
  VERIFY(square.determinant() != Scalar(0));

  // bug 219: MapAligned() was giving an assert with EIGEN_DONT_ALIGN, because Map Flags were miscomputed
  Scalar* array = internal::aligned_new<Scalar>(rows);
  v = VectorType::MapAligned(array, rows);
  internal::aligned_delete(array, rows);
}

void test_dontalign()
{
    #ifdef CYCLESCOUNT
    typedef unsigned int u32;
    u32 tick;
    #endif
// EIGEN_TEST_PART is defined in the makefile.
#if defined EIGEN_TEST_PART_1 || defined EIGEN_TEST_PART_5
     #ifdef CYCLESCOUNT
     tick = swcReadPC3();    
     #endif
  dontalign(Matrix3d());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(Matrix3d());] = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  dontalign(Matrix4f());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count[dontalign(Matrix4f());] = " << tick << endl;
     tick = swcReadPC3();
     #endif
     
#elif defined EIGEN_TEST_PART_2 || defined EIGEN_TEST_PART_6
     #ifdef CYCLESCOUNT
     tick = swcReadPC3();    
     #endif
  dontalign(Matrix3cd());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(Matrix3cd());] = " << tick << endl;
     tick = swcReadPC3();
     #endif
  dontalign(Matrix4cf());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(Matrix4cf());] = " << tick << endl;
     tick = swcReadPC3();
     #endif
     
#elif defined EIGEN_TEST_PART_3 || defined EIGEN_TEST_PART_7
     #ifdef CYCLESCOUNT
     tick = swcReadPC3();    
     #endif
  dontalign(Matrix<float, 32, 32>());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(Matrix<float, 32, 32>());] = " << tick << endl;
     tick = swcReadPC3();
     #endif
  dontalign(Matrix<std::complex<float>, 32, 32>());
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(Matrix<std::complex<float>, 32, 32>());] = " << tick << endl;
     tick = swcReadPC3();
     #endif
     
#elif defined EIGEN_TEST_PART_4 || defined EIGEN_TEST_PART_8
     #ifdef CYCLESCOUNT
     tick = swcReadPC3();    
     #endif
  dontalign(MatrixXd(32, 32));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(MatrixXd(32, 32));] = " << tick << endl;
     tick = swcReadPC3();
     #endif
  dontalign(MatrixXcf(32, 32));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count [dontalign(MatrixXcf(32, 32));] = " << tick << endl;
     tick = swcReadPC3();
     #endif
#endif
}
