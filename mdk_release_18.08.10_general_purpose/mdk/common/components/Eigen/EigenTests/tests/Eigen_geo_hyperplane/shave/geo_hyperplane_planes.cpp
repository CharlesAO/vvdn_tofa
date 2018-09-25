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

template<typename Scalar> void planes()
{
  using std::abs;
  typedef Hyperplane<Scalar, 3> Plane;
  typedef Matrix<Scalar,3,1> Vector;

  for(int i = 0; i < 10; i++)
  {
    Vector v0 = Vector::Random();
    Vector v1(v0), v2(v0);
    if(internal::random<double>(0,1)>0.25)
      v1 += Vector::Random();
    if(internal::random<double>(0,1)>0.25)
      v2 += v1 * std::pow(internal::random<Scalar>(0,1),internal::random<int>(1,16));
    if(internal::random<double>(0,1)>0.25)
      v2 += Vector::Random() * std::pow(internal::random<Scalar>(0,1),internal::random<int>(1,16));

    Plane p0 = Plane::Through(v0, v1, v2);

    VERIFY_IS_APPROX(p0.normal().norm(), Scalar(1));
    VERIFY_IS_MUCH_SMALLER_THAN(p0.absDistance(v0), Scalar(1));
    VERIFY_IS_MUCH_SMALLER_THAN(p0.absDistance(v1), Scalar(1));
    VERIFY_IS_MUCH_SMALLER_THAN(p0.absDistance(v2), Scalar(1));
  }
}



void test_geo_hyperplane_planes()
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
   CALL_SUBTEST_2( planes<float>() );
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif 
   CALL_SUBTEST_5( planes<double>() );
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     #endif 
  }
}
