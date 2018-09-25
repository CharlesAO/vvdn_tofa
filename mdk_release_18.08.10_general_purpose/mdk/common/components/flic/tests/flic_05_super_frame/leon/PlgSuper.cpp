///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin
///

#include <PlgSuper.h>
#include <ImgFrame.h>
#include <string.h>
#include <UnitTestApi.h>

//=============================================
void PlgSuper::Create()
{
   for(uint32_t i=0; i<N_PART; i++){
    iPart[i].Create(8);
    Add(&iPart[i]);
    Add(&oPart[i]);
   }

   Add(&iSuper);
   Add(&oSuper);
}

//=============================================
void * PlgSuper::threadFunc(void *)
{
    ImgFrame fTemp[N_PART]; //local info, not part of a pool !!!
    uint32_t i, partSz, run = 0;

    while(Alive())
    {
       ImgFramePtr    fSuper;
       ImgFramePtr    fPart[N_PART]; //floating frames (not part of a pool !!!)
       RefCnt          fCnt[N_PART];
       rtems_task_wake_after(300);

     //Get a big (super) buffer
       if(OK == iSuper.Receive(&fSuper))
       {
        //Setup sub-frame info and send to different consumer plugins
          partSz = fSuper.ptr->size / N_PART;
          for(i=0; i< N_PART; i++)
          {
            //=======================================================
            //hack so that ~PoPtr won't try to reinsert msg in Pool which doesn't exist
            fPart[i].ref       = &fCnt[i]; fCnt[i].cnt = 10;
            //=======================================================
            fPart[i].ptr       = &fTemp[i];
            fPart[i].ptr->base = (void*)((uint32_t)fSuper.ptr->base + i*partSz);
            fPart[i].ptr->size = partSz;
          }

        //Send to plugins that are to fill the SuperFrame
          for(i=0; i< N_PART; i++)
          {
            oPart[i].Send(&fPart[i]);
          }

        //Wait for completion:
          int err = 0;
          for(i=0; i< N_PART; i++)
           err += iPart[i].Receive(&fPart[i]);

          if(!err){
          //Checks (we know what each part sends for each run ...)
            unitTestAssert((*(uint8_t*)fPart[0].ptr->base) == (uint8_t)(pSeed[0] + run));
            unitTestAssert((*(uint8_t*)fPart[1].ptr->base) == (uint8_t)(pSeed[1] + run));

          //Send out the super frame:
            oSuper.Send(&fSuper);
          }
        }

        run++;
    }
    return NULL;
}