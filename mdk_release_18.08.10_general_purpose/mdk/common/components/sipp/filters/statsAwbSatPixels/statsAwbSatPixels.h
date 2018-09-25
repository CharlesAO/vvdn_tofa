#ifndef __StatsAWBSatPixels_H__
#define __StatsAWBSatPixels_H__

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
    UInt8    *statsOutput;
    UInt32   NextVerticalStartPos;
    UInt32   crtPosInPaxel;

}StatsAwbSatPixelsParam; 

void SVU_SYM(svuStatsAwbSatPixels)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuStatsAwbSatPixels);

#endif //StatsAWBSatPixels

