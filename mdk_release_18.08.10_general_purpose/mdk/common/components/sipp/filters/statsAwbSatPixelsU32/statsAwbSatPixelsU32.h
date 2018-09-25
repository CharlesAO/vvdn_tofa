#ifndef __StatsAWBSatPixelsU32_H__
#define __StatsAWBSatPixelsU32_H__

#include <sipp.h>


typedef struct
{
    struct {
        UInt32  firstPatchX;
        UInt32  firstPatchY;
        UInt32  patchWidth;
        UInt32  patchHeight;
        UInt32  patchGapX;
        UInt32  patchGapY;
        UInt32  nPatchesX;
        UInt32  nPatchesY;
        UInt32  satThresh;      /* 0.16 Fixed Point */
    } AeAwbStatsCfg;

    UInt32   runNr;
    UInt32   crtPaxel;
    UInt32    *statsOutput;
    UInt32   NextVerticalStartPos;
    UInt32   crtPosInPaxel;

}StatsAwbSatPixelsParamU32; 

void SVU_SYM(svuStatsAwbSatPixelsU32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuStatsAwbSatPixelsU32); 

#endif //StatsAWBSatPixels

