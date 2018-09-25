// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "main.h"
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <Eigen/QR>

#ifdef CYCLESCOUNT
#include "CyclesCount.h"
#endif

template<typename Scalar> void lines()
{
  using std::abs;
  typedef Hyperplane<Scalar, 2> HLine;
  typedef ParametrizedLine<Scalar, 2> PLine;
  typedef Matrix<Scalar,2,1> Vector;
  typedef Matrix<Scalar,3,1> CoeffsType;

  for(int i = 0; i < 10; i++)
  {
    Vector center = Vector::Random();
    Vector u = Vector::Random();
    Vector v = Vector::Random();
    Scalar a = internal::random<Scalar>();
    while (abs(a-1) < Scalar(1e-4)) a = internal::random<Scalar>();
    while (u.norm() < Scalar(1e-4)) u = Vector::Random();
    while (v.norm() < Scalar(1e-4)) v = Vector::Random();

    HLine line_u = HLine::Through(center + u, center + a*u);
    HLine line_v = HLine::Through(center + v, center + a*v);

    // the line equations should be normalized so that a^2+b^2=1
    VERIFY_IS_APPROX(line_u.normal().norm(), Scalar(1));
    VERIFY_IS_APPROX(line_v.normal().norm(), Scalar(1));

    Vector result = line_u.intersection(line_v);

    // the lines should intersect at the point we called "center"
    if(abs(a-1) > Scalar(1e-2) && abs(v.normalized().dot(u.normalized()))<Scalar(0.9))
      VERIFY_IS_APPROX(result, center);

    // check conversions between two types of lines
    PLine pl(line_u); // gcc 3.3 will commit suicide if we don't name this variable
    HLine line_u2(pl);
    CoeffsType converted_coeffs = line_u2.coeffs();
    if(line_u2.normal().dot(line_u.normal())<Scalar(0))
      converted_coeffs = -line_u2.coeffs();
    VERIFY(line_u.coeffs().isApprox(converted_coeffs));
  }
}




void test_geo_hyperplane_lines()
{
    #ifdef CYCLESCOUNT
    typedef unsigned int u32;
    u32 tick;
    g_repeat=1;
    #endif
    
  for(int i = 0; i < g_repeat; i++) {
     #ifdef CYCLESCOUNT
     tick = swcReadPC3();    
     #endif
   CALL_SUBTEST_1( lines<float>() );
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif 
   CALL_SUBTEST_3( lines<double>() ); 
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     #endif 
  
  }
}
