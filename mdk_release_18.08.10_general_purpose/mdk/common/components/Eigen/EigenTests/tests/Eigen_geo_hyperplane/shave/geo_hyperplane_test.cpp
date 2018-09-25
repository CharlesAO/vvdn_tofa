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


void test_geo_hyperplane(void); 
void test_geo_hyperplane_2(void); 
void test_geo_hyperplane_alignment(void); 
void test_geo_hyperplane_lines(void); 
void test_geo_hyperplane_planes(void); 

__attribute__((dllexport)) int main()
{
std::cerr.copyfmt(std::cout); 
std::cerr.clear(std::cout.rdstate()); 
std::cerr.rdbuf(std::cout.rdbuf());

 test_geo_hyperplane(); 
 test_geo_hyperplane_2();
 test_geo_hyperplane_alignment();
 test_geo_hyperplane_lines();
 test_geo_hyperplane_planes();
 
 printf("All Passed!\n");
 return 0;
}

