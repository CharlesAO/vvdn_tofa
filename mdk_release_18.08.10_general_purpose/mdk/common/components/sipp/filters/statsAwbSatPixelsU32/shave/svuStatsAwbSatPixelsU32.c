#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/statsAwbSatPixelsU32/statsAwbSatPixelsU32.h>

#ifdef SIPP_USE_MVCV
void mvispStatsAWBSatPixelsU32_asm(UInt32 *output, UInt16 *iline, UInt32 nPaxelX, UInt32 gapPaxelX, UInt32 widthPaxelX, UInt32 satPaxelX);
#else
//                              i18         i17         i16                 i15                 i14             i13
void statsAWBSatPixelsU32         (UInt32 *output, UInt16 *iline, UInt32 nPaxelX, UInt32 gapPaxelX, UInt32 widthPaxelX, UInt32 satPaxelX)
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
                output[crtP*2  ] = (output[crtP*2  ]+1);
            };
            if (startPosition[2*x+1]>satPaxelX)
            {
                output[crtP*2+1] = (output[crtP*2+1]+1);
            };
        }
    }
}
#endif

void svuStatsAwbSatPixelsU32(SippFilter *fptr)
{
    UInt16 *iline;
    StatsAwbSatPixelsParamU32* params = (StatsAwbSatPixelsParamU32*)fptr->params;
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
                memset((UInt32*)(params->statsOutput) + (params->crtPaxel*2) * params->AeAwbStatsCfg.nPatchesX * 2, 0, (params->AeAwbStatsCfg.nPatchesX*2*2) * 4);
                params->NextVerticalStartPos += params->AeAwbStatsCfg.patchGapY;
            }
            if(params->crtPosInPaxel < params->AeAwbStatsCfg.patchHeight)
            {
                params->crtPosInPaxel++;
                //callFunction
    #ifdef SIPP_USE_MVCV
                mvispStatsAWBSatPixelsU32_asm(
                    (UInt32*)((UInt32*)(params->statsOutput) + ((params->crtPaxel*2) + (params->runNr & 1)) * params->AeAwbStatsCfg.nPatchesX*2),
                    (UInt16*)((UInt32)iline + params->AeAwbStatsCfg.firstPatchX*2),
                    params->AeAwbStatsCfg.nPatchesX, params->AeAwbStatsCfg.patchGapX, params->AeAwbStatsCfg.patchWidth, params->AeAwbStatsCfg.satThresh);
    #else
                statsAWBSatPixelsU32(
                    (UInt32*)((UInt32*)(params->statsOutput) + ((params->crtPaxel*2) + (params->runNr & 1)) * params->AeAwbStatsCfg.nPatchesX*2),
                    (UInt16*)((UInt32)iline + params->AeAwbStatsCfg.firstPatchX*2),
                    params->AeAwbStatsCfg.nPatchesX, params->AeAwbStatsCfg.patchGapX, params->AeAwbStatsCfg.patchWidth, params->AeAwbStatsCfg.satThresh);
    #endif
                if(params->crtPosInPaxel == params->AeAwbStatsCfg.patchHeight)
                {
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
