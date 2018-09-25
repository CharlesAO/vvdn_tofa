#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/statsAwbSatPixels/statsAwbSatPixels.h>

#define CLAMPZ255(X) ((X)>255 ? 255 : ((X)<0 ? 0 : (X)))
//                              i18         i17         i16                 i15                 i14             i13
void statsAWBSatPixels(UInt8 *output, UInt16 *iline, UInt32 nPaxelX, UInt32 gapPaxelX, UInt32 widthPaxelX, UInt32 satPaxelX)
{
    Int32 x;
    UInt32 crtP;

    for(crtP = 0; crtP < nPaxelX; crtP++)
    {
        UInt16 *startPosition = (UInt16*)((crtP * gapPaxelX)*2 + (UInt32)iline);
        for(x=0; x<(int)(widthPaxelX>>1); x++)
        {
            if (startPosition[2*x  ]>satPaxelX)
            {
                output[crtP*2  ] = CLAMPZ255(output[crtP*2  ]+1);
            };
            if (startPosition[2*x+1]>satPaxelX)
            {
                output[crtP*2+1] = CLAMPZ255(output[crtP*2+1]+1);
            };
        }
    }
}

void svuStatsAwbSatPixels(SippFilter *fptr)
{
    UInt16 *iline;
    StatsAwbSatPixelsParam* params = (StatsAwbSatPixelsParam*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        iline=(UInt16*)getInPtr(fptr,0,iterNum,0,0);

        if (params->crtPaxel < params->AeAwbStatsCfg.nPatchesY)
        {
            if(params->runNr == params->NextVerticalStartPos)
            {
                params->crtPosInPaxel = 0;
                memset(params->statsOutput + (params->crtPaxel*2) * params->AeAwbStatsCfg.nPatchesX * 2, 0, params->AeAwbStatsCfg.nPatchesX*2*2);
                params->NextVerticalStartPos += params->AeAwbStatsCfg.patchGapY;
            }
            if(params->crtPosInPaxel < params->AeAwbStatsCfg.patchHeight)
            {
                params->crtPosInPaxel++;
                //callFunction
                statsAWBSatPixels(
                    (UInt8*)(params->statsOutput + ((params->crtPaxel*2) + (params->runNr & 1)) * params->AeAwbStatsCfg.nPatchesX*2),
                    (UInt16*)((UInt32)iline + params->AeAwbStatsCfg.firstPatchX*2),
                    params->AeAwbStatsCfg.nPatchesX, params->AeAwbStatsCfg.patchGapX, params->AeAwbStatsCfg.patchWidth, params->AeAwbStatsCfg.satThresh);
                if(params->crtPosInPaxel == params->AeAwbStatsCfg.patchHeight) {
                    params->crtPaxel++;
                }
            }
        }
        else
        {
            // reset parameters to default
        }
        params->runNr++;
    }
}
