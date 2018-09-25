///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test types/defs
///

#ifndef _CNN_FRAME_H_
#define _CNN_FRAME_H_

#include <ImgFrame.h>

#ifndef DESC_SIZE
#define DESC_SIZE 128
#endif

#define FathomStart() rtems_task_wake_after(100)
#define FathomWait()

class CnnFrame: public ImgFrame
{
  public:
    int age;
    int gender;
};

#endif