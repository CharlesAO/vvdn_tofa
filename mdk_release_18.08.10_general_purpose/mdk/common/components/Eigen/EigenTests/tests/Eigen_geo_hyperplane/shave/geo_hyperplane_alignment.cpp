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

template<typename Scalar> void hyperplane_alignment()
{
  typedef Hyperplane<Scalar,3,AutoAlign> Plane3a;
  typedef Hyperplane<Scalar,3,DontAlign> Plane3u;

  EIGEN_ALIGN_MAX Scalar array1[4];
  EIGEN_ALIGN_MAX Scalar array2[4];
  EIGEN_ALIGN_MAX Scalar array3[4+1];
  Scalar* array3u = array3+1;

  Plane3a *p1 = ::new(reinterpret_cast<void*>(array1)) Plane3a;
  Plane3u *p2 = ::new(reinterpret_cast<void*>(array2)) Plane3u;
  Plane3u *p3 = ::new(reinterpret_cast<void*>(array3u)) Plane3u;
  
  p1->coeffs().setRandom();
  *p2 = *p1;
  *p3 = *p1;

  VERIFY_IS_APPROX(p1->coeffs(), p2->coeffs());
  VERIFY_IS_APPROX(p1->coeffs(), p3->coeffs());
  
  #if defined(EIGEN_VECTORIZE) && EIGEN_MAX_STATIC_ALIGN_BYTES > 0
  if(internal::packet_traits<Scalar>::Vectorizable && internal::packet_traits<Scalar>::size<=4)
    VERIFY_RAISES_ASSERT((::new(reinterpret_cast<void*>(array3u)) Plane3a));
  #endif
}


void test_geo_hyperplane_alignment()
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
   CALL_SUBTEST_2( hyperplane_alignment<float>() );
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     #endif 
  }
}
