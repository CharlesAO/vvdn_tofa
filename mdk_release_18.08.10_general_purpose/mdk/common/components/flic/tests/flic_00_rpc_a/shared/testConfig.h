///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test params
///

#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

#if 1
  #define N          4 //number of barriers
  #define RUNS     100 //number of test loops
  #define SLEEP   5000 //us
  #define SDELTA    10 //us
  #define PRINT      1
#elif 0
  #define N          6 //number of barriers
  #define RUNS   50000 //number of test loops
  #define SLEEP      0 //us
  #define SDELTA     0 //us
  #define PRINT      0
#endif

void test_0(RmtBarr *barArr);
void test_1(RmtBarr *barArr);

#endif