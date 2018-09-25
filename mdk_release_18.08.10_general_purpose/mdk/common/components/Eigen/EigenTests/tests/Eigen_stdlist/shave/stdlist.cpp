// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2010 Hauke Heibel <hauke.heibel@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "main.h"
#include <Eigen/StdList>
#include <Eigen/Geometry>

#ifdef CYCLESCOUNT
#include "CyclesCount.h"
#endif

template<typename MatrixType>
void check_stdlist_matrix(const MatrixType& m)
{
  typedef typename MatrixType::Index Index;
  
  Index rows = m.rows();
  Index cols = m.cols();
  MatrixType x = MatrixType::Random(rows,cols), y = MatrixType::Random(rows,cols);
  std::list<MatrixType,Eigen::aligned_allocator<MatrixType> > v(10, MatrixType(rows,cols)), w(20, y);
  v.front() = x;
  w.front() = w.back();
  VERIFY_IS_APPROX(w.front(), w.back());
  v = w;

  typename std::list<MatrixType,Eigen::aligned_allocator<MatrixType> >::iterator vi = v.begin();
  typename std::list<MatrixType,Eigen::aligned_allocator<MatrixType> >::iterator wi = w.begin();
  for(int i = 0; i < 20; i++)
  {
    VERIFY_IS_APPROX(*vi, *wi);
    ++vi;
    ++wi;
  }

  v.resize(21);  
  v.back() = x;
  VERIFY_IS_APPROX(v.back(), x);
  v.resize(22,y);
  VERIFY_IS_APPROX(v.back(), y);
  v.push_back(x);
  VERIFY_IS_APPROX(v.back(), x);
}

template<typename TransformType>
void check_stdlist_transform(const TransformType&)
{
  typedef typename TransformType::MatrixType MatrixType;
  TransformType x(MatrixType::Random()), y(MatrixType::Random());
  std::list<TransformType,Eigen::aligned_allocator<TransformType> > v(10), w(20, y);
  v.front() = x;
  w.front() = w.back();
  VERIFY_IS_APPROX(w.front(), w.back());
  v = w;

  typename std::list<TransformType,Eigen::aligned_allocator<TransformType> >::iterator vi = v.begin();
  typename std::list<TransformType,Eigen::aligned_allocator<TransformType> >::iterator wi = w.begin();
  for(int i = 0; i < 20; i++)
  {
    VERIFY_IS_APPROX(*vi, *wi);
    ++vi;
    ++wi;
  }

  v.resize(21);
  v.back() = x;
  VERIFY_IS_APPROX(v.back(), x);
  v.resize(22,y);
  VERIFY_IS_APPROX(v.back(), y);
  v.push_back(x);
  VERIFY_IS_APPROX(v.back(), x);
}

template<typename QuaternionType>
void check_stdlist_quaternion(const QuaternionType&)
{
  typedef typename QuaternionType::Coefficients Coefficients;
  QuaternionType x(Coefficients::Random()), y(Coefficients::Random());
  std::list<QuaternionType,Eigen::aligned_allocator<QuaternionType> > v(10), w(20, y);
  v.front() = x;
  w.front() = w.back();
  VERIFY_IS_APPROX(w.front(), w.back());
  v = w;

  typename std::list<QuaternionType,Eigen::aligned_allocator<QuaternionType> >::iterator vi = v.begin();
  typename std::list<QuaternionType,Eigen::aligned_allocator<QuaternionType> >::iterator wi = w.begin();
  for(int i = 0; i < 20; i++)
  {
    VERIFY_IS_APPROX(*vi, *wi);
    ++vi;
    ++wi;
  }

  v.resize(21);
  v.back() = x;
  VERIFY_IS_APPROX(v.back(), x);
  v.resize(22,y);
  VERIFY_IS_APPROX(v.back(), y);
  v.push_back(x);
  VERIFY_IS_APPROX(v.back(), x);
}

void test_stdlist()
{
    #ifdef CYCLESCOUNT
    typedef unsigned int u32;
    u32 tick;
    tick = swcReadPC3();  
    #endif
  // some non vectorizable fixed sizes
  CALL_SUBTEST_1(check_stdlist_matrix(Vector2f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_1(check_stdlist_matrix(Matrix3f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_2(check_stdlist_matrix(Matrix3d()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  

  // some vectorizable fixed sizes
  CALL_SUBTEST_1(check_stdlist_matrix(Matrix2f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_1(check_stdlist_matrix(Vector4f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_1(check_stdlist_matrix(Matrix4f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_2(check_stdlist_matrix(Matrix4d()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  

  // some dynamic sizes
  CALL_SUBTEST_3(check_stdlist_matrix(MatrixXd(1,1)));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_3(check_stdlist_matrix(VectorXd(20)));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_3(check_stdlist_matrix(RowVectorXf(20)));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_3(check_stdlist_matrix(MatrixXcf(10,10))); 
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  

  // some Transform
  CALL_SUBTEST_4(check_stdlist_transform(Affine2f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_4(check_stdlist_transform(Affine3f()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_4(check_stdlist_transform(Affine3d()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  

  // some Quaternion
  CALL_SUBTEST_5(check_stdlist_quaternion(Quaternionf()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif  
  CALL_SUBTEST_5(check_stdlist_quaternion(Quaterniond()));
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     #endif  
}
