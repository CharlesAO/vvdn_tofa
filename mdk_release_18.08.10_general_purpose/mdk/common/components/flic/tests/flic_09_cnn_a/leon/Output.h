///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy output
///

#ifndef __PLG_OUT_H__
#define __PLG_OUT_H__

#include <Flic.h>
#include <Receiver.h>
#include <UnitTestApi.h>

#include "DetAge.h"
#include "DetGen.h"

class PlgOut : public ThreadedPlugin
{
  public:
    SReceiver<CnnFramePtr> in;

    void Create(){
       in.Create(4);
       Add(&in);
    }

    void *threadFunc(void *)
    {
        while(Alive())
        {
            CnnFramePtr fMsg;
            if(OK == in.Receive(&fMsg))
            {
              CnnFrame *f = fMsg.ptr;
              printf("Out: descSize = %ld at %lx\n", f->descPos, (uint32_t)f->descBuf);

             //Analyze descriptor (TBD: should be robust to plugs order)
              unitTestAssert(f->descPos == sizeof(AgeInfo) + sizeof(GenInfo));
              uint8_t *ptr = f->descBuf;
              if(OK == strncmp((const char*)ptr, AGE_STR, strlen(AGE_STR)))
              {
                  AgeInfo *ai = (AgeInfo*)ptr;
                  unitTestAssert(ai->age == AGE_VAL);
                  ptr += sizeof(AgeInfo);//we're done with AgeInfo
              }
              if(OK == strncmp((const char*)ptr, GEN_STR, strlen(GEN_STR)))
              {
                  GenInfo *gi = (GenInfo*)ptr;
                  unitTestAssert(gi->gender == GEN_VAL);
                  ptr += sizeof(GenInfo);//we're done with GenInfo
              }
            }
        }
        return NULL;
    }
};

#endif