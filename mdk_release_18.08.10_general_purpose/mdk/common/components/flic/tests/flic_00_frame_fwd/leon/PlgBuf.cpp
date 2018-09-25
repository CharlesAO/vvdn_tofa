///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy buffer plugin
///

#include "PlgBuf.h"
#include <ImgFrame.h>

//#############################################
void PlgBuf::Create()
{
    in.Create(4);
    Add(&in); Add(&out);
}
//=============================================
void * PlgBuf::threadFunc(void *)
{
    while(Alive())
    {
        ImgFramePtr frm;
        if(OK == in.Receive(&frm)){
          out.Send  (&frm);
        }
    }
    return NULL;
}