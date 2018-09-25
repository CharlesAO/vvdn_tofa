///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup 3A plugin
///

#ifndef __PLG_AAA_H__
#define __PLG_AAA_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>

class PlgAaa : public ThreadedPlugin
{
  public:
    void *threadFunc(void *);
    void  Create();
};

#endif