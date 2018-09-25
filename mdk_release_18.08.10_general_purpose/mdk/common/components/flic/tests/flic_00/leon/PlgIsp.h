///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup ISP plugin
///

#ifndef __PLG_ISP_H__
#define __PLG_ISP_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>

class PlgIsp : public ThreadedPlugin
{
  public:
    void *threadFunc(void *);
    void  Create();
};

#endif