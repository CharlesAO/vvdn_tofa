///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <string.h>
#include <ImgFrame.h>

//##################################################################
void PlgEchoSend::Create()
{
    in.Create(8);
    Add(&in);Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    while(Alive())
    {
      ImgFramePtr fMsg; //from mem pool
     //1)get a frame from mempool
      if(OK == in.Receive(&fMsg))
      {
       //2)change frame data (as Camera would)
        memset(fMsg.ptr->base, seed, fMsg.ptr->size);
        seed++;
       //3)Send data to consumer
        out.Send(&fMsg);
      }
    }
    return NULL;
}

//##################################################################
void PlgEchoRecv::Create()
{
    in.Create(8);
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    while(Alive())
    {
        ImgFramePtr fMsg;
        if(OK == in.Receive(&fMsg))
        {
           //whole frame is 128 bytes, display 1st byte of each side
           //tbd... do this right...
             printf("Part1=%3x Part2=%3x\n", ((uint8_t*)fMsg.ptr->base)[0 ],
                                             ((uint8_t*)fMsg.ptr->base)[64]);
        }
    }
    return NULL;
}