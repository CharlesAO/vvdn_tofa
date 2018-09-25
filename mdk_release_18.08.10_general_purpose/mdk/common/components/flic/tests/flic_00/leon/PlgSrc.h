///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy test source
///

#ifndef __PLG_SRC_H__
#define __PLG_SRC_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>

class PlgSrc : public ThreadedPlugin
{
  public:
    void    *threadFunc(void *);
    void     Create();
};

#endif