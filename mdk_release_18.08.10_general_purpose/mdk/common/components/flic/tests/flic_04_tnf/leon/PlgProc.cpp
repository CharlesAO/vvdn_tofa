///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup TNF plugin
///

#include <PlgProc.h>
#include <ImgFrame.h>
#include <string.h>

void PlgProc::Create()
{
    inI.Create(4);
    Add(&inI);Add(&inO);Add(&out);
}

//=============================================
void * PlgProc::threadFunc(void *)
{
    int err = 0;
  //Temp refs (0: oldest, 2: newest)
    ImgFramePtr iTmpBuff[3];

    while(Alive())
    {
        ImgFramePtr fInp;
        ImgFramePtr fOut;

        err += inI.Receive(&fInp);
        err += inO.Receive(&fOut);

        if(!err)
        {
            iTmpBuff[0] = iTmpBuff[1]; //oldest gets released here
            iTmpBuff[1] = iTmpBuff[2];
            iTmpBuff[2] = fInp       ;

          //Out = some "avg" of input !!!
            uint32_t avg = 0;
            uint8_t  cnt = 0;
            if(iTmpBuff[0].ptr) {cnt++; avg += ((uint8_t*)iTmpBuff[0].ptr->base)[0];}
            if(iTmpBuff[1].ptr) {cnt++; avg += ((uint8_t*)iTmpBuff[1].ptr->base)[0];}
            if(iTmpBuff[2].ptr) {cnt++; avg += ((uint8_t*)iTmpBuff[2].ptr->base)[0];}
            avg /= cnt;
            memset(fOut.ptr->base, avg, fOut.ptr->size);

          //Can optionally release oldest input early, as we won't need it any more.
          //If we skip this step, then oldest gets released only later, when new input
          //is available (see line 27)
            if(iTmpBuff[0].ptr){
               iTmpBuff[0] = nullptr;
            }

            out.Send(&fOut); //FWD to consumer
        }
    }//fInp, fOut released automatically here
    return NULL;
}