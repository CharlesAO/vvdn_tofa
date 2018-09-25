///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "Plugs.h"
#include <UnitTestApi.h>

#define POSX(i) (10+i)
#define POSY(i) (20+i)
#define WIDT(i) (30+i)
#define HGHT(i) (40+i)

//############################################################
void PlgIsp::Create(){
    Add(&inO, ".iYuv");
    Add(&out, ".oYuv");
}
//=============================================
void * PlgIsp::threadFunc(void *)
{
    uint32_t  nFrames = 0;

    while(Alive())
    {
      PoPtr<RoiFrame>  fOut;
     //Assume we got from source a RAW frame and
     //now we need a free yuv output to fill
      if(OK == inO.Receive(&fOut))
      {
        //Isp writes fOut->frm, mimic processing time
        //via a delay
        rtems_task_wake_after(300);
      //printf("[PlgIsp] send yuv\n");
        //YUV frame ready, fwd to consumers:
        out.Send(&fOut);

        nFrames++;
        if(nFrames == 8)
          break;
      }
    }
    return NULL;
}

//############################################################
void PlgFaceDet::Create()
{
    in.Create(4);
    Add(&in , ".inImg");
    Add(&out, ".oRoi");
}
//=============================================
void * PlgFaceDet::threadFunc(void *)
{
    uint32_t  k = 0;

    while(Alive())
    {
        PoPtr<RoiFrame> fIn;
        PoPtr<RoiFrame> rm;

        //1) Receive an Image frame
        if(OK == in.Receive(&fIn))
        {
         //printf("[PlgFaceDet] Got frame\n");
           //2)Mimic roi detection for current frame:
           for(uint32_t i=0; i<3; i++)
           {
             rtems_task_wake_after(5);
             rm = fIn;
             rm->roi.x = POSX(k); rm->roi.y = POSY(k);
             rm->roi.w = WIDT(k); rm->roi.h = HGHT(k);
             //log->Print("[PlgFaceDet] found roi %d %d %d %d\n",
             //           rm.roi.x, rm.roi.y, rm.roi.w, rm.roi.h);
             out.Send(&rm);
             k++;
           }
        }
    }//fIn, rm automatically released here
    return NULL;
}

//############################################################
void PlgOut::Create()
{
    in.Create(4);
    Add(&in, ".inRoi");
}
//=============================================
void * PlgOut::threadFunc(void *)
{
    uint32_t k = 0;

    while(Alive())
    {
        PoPtr<RoiFrame> rm;
        if(OK == in.Receive(&rm))
        {
          //1)Encode/display the region
        //printf("[PlgOut] encoding roi\n");
          //Basic Checks
          unitTestAssert(rm->roi.x == POSX(k));
          unitTestAssert(rm->roi.y == POSY(k));
          unitTestAssert(rm->roi.w == WIDT(k));
          unitTestAssert(rm->roi.h == HGHT(k));
          k++;
        }
    }//'rm' relaesed here automatically
    return NULL;
}