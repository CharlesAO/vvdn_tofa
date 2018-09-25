///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy sink plugin
///

#include "PlgRecv.h"
#include <ImgFrame.h>
#include "cfg.h"

//#############################################
void PlgRecv::Create()
{
    in.Create(4);
    Add(&in);
    no = 0;
}
//=============================================
void * PlgRecv::threadFunc(void *)
{
    for(int i=0; i<T_FRM; i++)
    {
        ImgFramePtr fMsg;
        if(OK == in.Receive(&fMsg)){
           printf("PlgRecv [%2d]\n", no);
           no++;
        }
    }
  //Invoke pipeline teardown
    pl->Stop();

    return NULL;
}