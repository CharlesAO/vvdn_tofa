// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2015 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include <stdlib.h>
#include <stdio.h>
#include <sys/shave_system.h>
#include <iostream>
using namespace std;



void benchAATProduct(void);
void benchATAProduct(void);
void benchATVProduct(void);
void benchMatrixVectorProduct(void);
void benchMatrixMatrixProduct(void); 
void benchAXPBY(void);
void benchAXPY(void);
void benchGer(void);
void benchRot(void);
void benchSymv(void);
void benchSyr2(void);
void benchHessengerb(void);
void benchCholesky(void);
void benchLuDecomp(void);
void benchPartialLu(void);
void benchTrisolve(void);
void benchTrisolveMatrix(void);

__attribute__((dllexport)) int main()
{


    benchAATProduct();
    benchATAProduct();
    benchATVProduct();
    benchMatrixVectorProduct();
    benchMatrixMatrixProduct(); 
    benchAXPBY();
    benchAXPY();
    benchGer();
    benchRot();
    benchSymv();
    benchSyr2();
    benchHessengerb();
    benchCholesky();
    benchLuDecomp();
    benchPartialLu();
    benchTrisolve();
    benchTrisolveMatrix();
       
       
 printf("\n Finished Benchmark \n");
 return 0;
}
