///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin
///

#ifndef __PLG_SUPER_H__
#define __PLG_SUPER_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

#define N_PART 2

class PlgSuper: public ThreadedPlugin
{
  public:
    MReceiver<ImgFramePtr> iSuper;    //super-input from mempool
    MSender  <ImgFramePtr> oSuper;    //out msg to consumers

    MSender  <ImgFramePtr> oPart[N_PART];
    SReceiver<ImgFramePtr> iPart[N_PART]; //in [Input frame ]-read only

    uint32_t  pSeed[N_PART]; //part seed

    virtual void *threadFunc(void *param);
    virtual void Create();
};

#endif