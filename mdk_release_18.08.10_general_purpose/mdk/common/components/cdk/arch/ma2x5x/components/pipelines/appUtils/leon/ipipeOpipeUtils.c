#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "ipipeUtils.h"
#include "ipipeOpipeUtils.h"
//Related plugins
#include "PlgSourceApi.h"
#include "IspCommonUtils.h"

// CMX buffer pool for Opipe CMX-Circular buffers
#ifndef OPIPE_CBUFF_SZ
#define OPIPE_CBUFF_SZ (700*1024)
#endif

#ifndef OPIPE_CBUFF_SECT
#define OPIPE_CBUFF_SECT ".cmx.bss"
#endif

static uint8_t   opipeCircBuff[OPIPE_CBUFF_SZ] SECTION(OPIPE_CBUFF_SECT) ALIGNED(8); //691KB
static MemPoolId memId = NO_VALID_MEM;
static uint32_t  maxReq [24]; //max ISP output buffer mem requirement [bytes]

//###############################################################################
MemPoolId CheckAllocOmemPool()
{
  if(memId == NO_VALID_MEM)
     memId =  MemMgrAddPool(opipeCircBuff, sizeof(opipeCircBuff));
  return memId;
}

//###############################################################################
static void trackMaxReq(uint32_t *newReq)
{
   uint32_t i;

   if(newReq == NULL){
     for(i=0; i<24; i++)
      maxReq[i] = 0;
   }
   else{
     for(i=0; i<24; i++){
      if(maxReq[i]<newReq[i])
         maxReq[i]=newReq[i];
     }
   }
}

//###############################################################################
static void AllocMaxReq()
{
   uint32_t id; //sipp HW filter ID
   CheckAllocOmemPool();

   for(id=0; id<24; id++){
    if(maxReq[id])
     ogcBuff[id] = (uint8_t*)MemMgrAlloc(memId, maxReq[id]);
   }
}

//###############################################################################
//For tear down
void AllocOpipeReset()
{
   memId = NO_VALID_MEM;
   trackMaxReq(NULL); //initial max clear
}

//###############################################################################
//Alloc CMX circular buffer for sippRx plugins (one per instance)
void AllocOpipeRxCmxBuffs(icCtrl *ctrl)
{
    uint32_t i;

    CheckAllocOmemPool();

    for(i=0; i<IPIPE_MAX_SOURCES_ALLOWED; i++)
     if(IPIPE_SRC_SETUP == ctrl->source[i].sourceStatus)
       if(ctrl->source[i].sourceConfig.mipiRxData.recNrl <= IC_SIPP_DEVICE3) //if a SippRx source
         PlgSourceCmxAlloc(i, &ctrl->source[i].sourceSetup, memId);
}

//###############################################################################
static void GetSourceWidths(icCtrl *ctrl, int s, uint32_t *_szIn, uint32_t *_szMain, uint32_t *_szOut)
{
  //assert(IPIPE_SRC_SETUP == ctrl->source[s].sourceStatus);
    icSourceSetup *src     = &ctrl->source[s].sourceSetup;

    assert(src->maxHorizD != 0);//div by zero !
    assert(IPIPE_SRC_SETUP == ctrl->source[s].sourceStatus);

    *_szIn   = lineWidthInBytes(src->maxWidth, src->maxBpp);
    *_szMain = src->maxWidth;
    *_szOut  = (src->maxWidth * src->maxHorizN - 1)/src->maxHorizD+1;
}

//###############################################################################
void AllocOpipeIspCmxBuffs(icCtrl *ctrl, uint32_t maxCams, PlgIspBase *ispPlg[])
{
   uint32_t x;

   for(x=0; x<maxCams; x++)
   {
     uint32_t wSig, wMain, wPoly;
     uint32_t sz[24]; //current
     if(IPIPE_SRC_SETUP == ctrl->source[x].sourceStatus)
     {
       memset(sz, 0, sizeof(sz)); //clear for each
       GetSourceWidths(ctrl, x, &wSig, &wMain, &wPoly);
       ispPlg[x]->fnQueryMemReq(ispPlg[x], wSig, wMain, wPoly, sz);
       trackMaxReq(sz);
     }
   }
   AllocMaxReq();
}

//###############################################################################
//Allocate circular buffs considering a single pipe needs
//(useful for standalone single/pipe or multiple symmetrical pipe tests)
void AllocIspBuffs(PlgIspBase *base, uint32_t inW, uint32_t mainW, uint32_t outW)
{
    memset(maxReq, 0, sizeof(maxReq)); //clear requirements
    base->fnQueryMemReq(base, inW, mainW, outW, maxReq);

    CheckAllocOmemPool();
    AllocMaxReq();
}

//###############################################################################
//Setup source maximal params (needed for circular buffer mem allocs)
//LOS would do this typically; this is used in LRT standalone tests typically
void dbgSetSourceSetup(icCtrl *ctrl, uint32_t no, uint32_t maxW, uint32_t maxB)
{
   icSourceSetup ss;
   ss.maxBpp   = maxB;
   ss.maxWidth = maxW;

  //Mark the configuration:
   ctrl->source[no].sourceStatus = IPIPE_SRC_SETUP;
   ctrl->source[no].sourceSetup  = ss;
}

//###############################################################################
uint32_t GetFrameBppPackFormat(uint32_t bpp)
{
  switch (bpp)
  {
    case 10 :            return SIPP_FMT_PACK10;
    case 12 :            return SIPP_FMT_PACK12;
    default : if(bpp>16) return SIPP_FMT_32BIT;
              if(bpp> 8) return SIPP_FMT_16BIT;
              if(bpp<=8) return SIPP_FMT_8BIT;
  }
  return 0; //should not reach this !
}
