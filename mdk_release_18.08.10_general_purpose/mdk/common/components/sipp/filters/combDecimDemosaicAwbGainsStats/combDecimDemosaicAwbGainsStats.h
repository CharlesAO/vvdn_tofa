#ifndef  __CombDecimDemosaic_H__
#define __CombDecimDemosaic_H__

#include <sipp.h>

    typedef struct {
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

typedef struct
{
    AeAwbStatsCfg  *satPixelsStats;
    UInt32         runNr;
    unsigned int   *statsOutput;
    unsigned short *paxelsIntervalsVert;
    unsigned short *paxelsIntervalsHz;
    unsigned int   crtPaxelLine;
    unsigned int   gains[3];
}CombDecimStatsGainsParam; 

void SVU_SYM(svuCombDecimDemosaicAwbGainsStats)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCombDecimDemosaicAwbGainsStats);

#endif

