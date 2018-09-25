///
/// @file      IspMsg.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IspMsg header.
///

#ifndef __ISP_MSG_H__
#define __ISP_MSG_H__

#include "ipipe.h"
#include <Flic.h>
#include "ImgFrame.h"
#include "IspCommon.h"

class IspCmdMsg
{
public:
    icEvent      cmdMsg;
};
class IspInputMsg
{
  public:
    icIspConfig*  ispCfg;
    ImgFramePtr   img;
};

#endif //__ISP_MSG_H__
