///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Common structures used by both Shave and Leon

#ifndef __COMMON__H_
#define __COMMON__H_

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define TASKS 5
#define BUFFSIZE 1024
#define NUMBER_OF_SLAVES_USED 11

typedef struct MyData{
   int n;
   int data[NUMBER_OF_SLAVES_USED][BUFFSIZE];
}MyDataType;

#endif
