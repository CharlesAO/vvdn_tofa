#ifndef __extStatsSatPixelsU32_H__
#define __extStatsSatPixelsU32_H__

#include <sipp.h>

//======================== Stats Saturated Pixels=======================================
typedef struct {
    UInt32      mask;
    UInt32      runNr;
    struct
    {
        UInt32 base;     //+0x0000 : base address
        UInt32 cfg;      //+0x0004 : configuration
        UInt32 ls;       //+0x0008 : line stride
        UInt32 ps;       //+0x000C : plane stride
        UInt32 irqRate;  //+0x0010 : fileds for Chunk-size !
        UInt32 fillCtrl; //+0x0014 : unused in sync mode
        UInt32 ctx;      //+0x0018 : context/status
    }in;
    UInt8*      outStatsBuffer;
    UInt8*      outStatsBufferOutside;

    UInt32      crtPaxel;
    UInt32      nextVerticalStartPos;
    UInt32      crtPosInPaxel;

    UInt32      firstPatchX;
    UInt32      firstPatchY;
    UInt32      patchWidth;
    UInt32      patchHeight;
    UInt32      patchGapX;
    UInt32      patchGapY;
    UInt32      nPatchesX;
    UInt32      nPatchesY;
    UInt32      satThresh;
    void        (*statsSaturatePxl)(void *);
    UInt32      shaveNr;
    UInt32      stackPointer;
}ExtStatsSatPixelsU32Param;

void SVU_SYM(svuExtStatsSatPixelsU32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuExtStatsSatPixelsU32);

#endif //__extStatsSatPixelsU32_H__

