///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy test output
///

#ifndef __PLG_OUT_H__
#define __PLG_OUT_H__

#include <Flic.h>
#include <Receiver.h>

class PlgOut : public ThreadedPlugin
{
  public:
    void  *threadFunc(void *);
    void   Create();
};

#endif