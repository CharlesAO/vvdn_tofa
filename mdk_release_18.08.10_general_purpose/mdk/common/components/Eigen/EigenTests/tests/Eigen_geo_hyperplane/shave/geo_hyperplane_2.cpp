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

template<typename HyperplaneType> void hyperplane(const HyperplaneType& _plane)
{
  /* this test covers the following files:
     Hyperplane.h
  */
  using std::abs;
  typedef typename HyperplaneType::Index Index;
  const Index dim = _plane.dim();
  enum { Options = HyperplaneType::Options };
  typedef typename HyperplaneType::Scalar Scalar;
  typedef typename HyperplaneType::RealScalar RealScalar;
  typedef Matrix<Scalar, HyperplaneType::AmbientDimAtCompileTime, 1> VectorType;
  typedef Matrix<Scalar, HyperplaneType::AmbientDimAtCompileTime,
                         HyperplaneType::AmbientDimAtCompileTime> MatrixType;

  VectorType p0 = VectorType::Random(dim);
  VectorType p1 = VectorType::Random(dim);

  VectorType n0 = VectorType::Random(dim).normalized();
  VectorType n1 = VectorType::Random(dim).normalized();

  HyperplaneType pl0(n0, p0);
  HyperplaneType pl1(n1, p1);
  HyperplaneType pl2 = pl1;

  Scalar s0 = internal::random<Scalar>();
  Scalar s1 = internal::random<Scalar>();

  VERIFY_IS_APPROX( n1.dot(n1), Scalar(1) );

  VERIFY_IS_MUCH_SMALLER_THAN( pl0.absDistance(p0), Scalar(1) );
  if(numext::abs2(s0)>RealScalar(1e-6))
    VERIFY_IS_APPROX( pl1.signedDistance(p1 + n1 * s0), s0);
  else
    VERIFY_IS_MUCH_SMALLER_THAN( abs(pl1.signedDistance(p1 + n1 * s0) - s0), Scalar(1) );
  VERIFY_IS_MUCH_SMALLER_THAN( pl1.signedDistance(pl1.projection(p0)), Scalar(1) );
  VERIFY_IS_MUCH_SMALLER_THAN( pl1.absDistance(p1 +  pl1.normal().unitOrthogonal() * s1), Scalar(1) );

  // transform
  if (!NumTraits<Scalar>::IsComplex)
  {
    MatrixType rot = MatrixType::Random(dim,dim).householderQr().householderQ();
    DiagonalMatrix<Scalar,HyperplaneType::AmbientDimAtCompileTime> scaling(VectorType::Random());
    Translation<Scalar,HyperplaneType::AmbientDimAtCompileTime> translation(VectorType::Random());
    
    while(scaling.diagonal().cwiseAbs().minCoeff()<RealScalar(1e-4)) scaling.diagonal() = VectorType::Random();

    pl2 = pl1;
    VERIFY_IS_MUCH_SMALLER_THAN( pl2.transform(rot).absDistance(rot * p1), Scalar(1) );
    pl2 = pl1;
    VERIFY_IS_MUCH_SMALLER_THAN( pl2.transform(rot,Isometry).absDistance(rot * p1), Scalar(1) );
    pl2 = pl1;
    VERIFY_IS_MUCH_SMALLER_THAN( pl2.transform(rot*scaling).absDistance((rot*scaling) * p1), Scalar(1) );
    pl2 = pl1;
    VERIFY_IS_MUCH_SMALLER_THAN( pl2.transform(rot*scaling*translation)
                                  .absDistance((rot*scaling*translation) * p1), Scalar(1) );
    pl2 = pl1;
    VERIFY_IS_MUCH_SMALLER_THAN( pl2.transform(rot*translation,Isometry)
                                 .absDistance((rot*translation) * p1), Scalar(1) );
  }

  // casting
  const int Dim = HyperplaneType::AmbientDimAtCompileTime;
  typedef typename GetDifferentType<Scalar>::type OtherScalar;
  Hyperplane<OtherScalar,Dim,Options> hp1f = pl1.template cast<OtherScalar>();
  VERIFY_IS_APPROX(hp1f.template cast<Scalar>(),pl1);
  Hyperplane<Scalar,Dim,Options> hp1d = pl1.template cast<Scalar>();
  VERIFY_IS_APPROX(hp1d.template cast<Scalar>(),pl1);
}



void test_geo_hyperplane_2()
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
   CALL_SUBTEST_3( hyperplane(Hyperplane<double,4>()) ); 
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     tick = swcReadPC3();
     #endif 
   CALL_SUBTEST_4( hyperplane(Hyperplane<std::complex<double>,5>()) );
     #ifdef CYCLESCOUNT
     tick = swcReadPC3()-tick;
     cout << "Cycles count = " << tick << endl;
     #endif 
   
  }
}
