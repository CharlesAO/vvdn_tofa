// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.

#include <stdlib.h>
#include <stdio.h>
#include <sys/shave_system.h>
#include <iostream>
using namespace std;


void test_packetmath(void); 

__attribute__((dllexport)) int main()
{

 test_packetmath(); 
 
 printf("All Passed !\n");
 return 0;
}


