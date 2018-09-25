#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/combDecimDemosaicAwbGainsStats/combDecimDemosaicAwbGainsStats.h>

// declare an empty byffer, with 0 inside;
#define MAX_STATS_SIZE (68 * 4)

UInt32 emptyBuf[MAX_STATS_SIZE];
UInt32 DisablePaxelSumMacro = 0xFFFFFFFF;

void combDecimDemosaicFinal
    (   unsigned char **output,// i18 //output RGB with gain applied pixels
        UInt32 *outStatSat,    // i17 //statistics output
        unsigned short **iline,// i16 //input GRBG image
        unsigned int width,    // i15 //output width
        unsigned int gains[3], // i14 //AWB gains fixed point 8.8 precision
        UInt32 satThresh,      // i13 //
        UInt16  *indexListHz,  // i12 //list containing for every paxel first and last position in the line
        UInt32 clearBuff       // i11 //clear buffer, if is first line in paxel
    )
{
    int i;
    iline[1][-1] = iline[1][1];
    iline[3][-1] = iline[3][1];

    UInt16 pixR;
    UInt16 pixG;
    UInt16 pixB;

    UInt32 *inStatLine;

    UInt32 enableStats = 0;
    if (clearBuff) {
        inStatLine = emptyBuf;
    }
    else {
        inStatLine = outStatSat;
    }

    UInt32 *outSat = outStatSat;

    UInt16 *stsHzLst = indexListHz;
    UInt32 sumSat;
    UInt32 sumIter = 0;
    UInt32 itrCont = 0;

    for(i=0; i<(int)width; i++)
    {
        sumIter = 0;
        // all the time calculations, no time consumming hopefully
        if (iline[1][i*2+0] > satThresh) sumIter++; // cmu.cmvv
        if (iline[1][i*2+1] > satThresh) sumIter++; // cmu.cpti i0, C_CMU0
        if (iline[2][i*2+0] > satThresh) sumIter++; // iau.ones i1, i0
        if (iline[2][i*2+1] > satThresh) sumIter++; // iau.add i2, i2, i1

        // decision part can be consumming, hopefully not significant
        if(itrCont == stsHzLst[0]) {
              sumSat = *inStatLine; // load
              inStatLine++; // increment address
              enableStats = 1;
        }
        itrCont +=2;
        if (enableStats) {
            sumSat += sumIter;
        }
        if(itrCont == stsHzLst[1]) {
            stsHzLst+=2;
            *outSat = sumSat;
            outSat++;
            enableStats = 0;
        }

        pixR =   ((iline[1][i*2-1] * 3 +  // R
                           iline[1][i*2+1] * 9 +
                           iline[3][i*2-1] * 1 +
                           iline[3][i*2+1] * 3) >> 4 );

        pixG =   ((iline[1][i*2  ] +  // G
                           iline[2][i*2+1] + 1) >> 1);

        pixB =   ((iline[0][i*2  ] * 3 +  // B
                           iline[0][i*2+2] * 1 +
                           iline[2][i*2  ] * 9 +
                           iline[2][i*2+2] * 3) >> 4);
        // applay gains
        output[0][i] = ((pixR * gains[0]) >> 8)>>2; // 2 instructions / 4
        output[1][i] = ((pixG * gains[1]) >> 8)>>2; // 2 instructions / 4
        output[2][i] = ((pixB * gains[2]) >> 8)>>2; // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];

    // if last paxel have exactly same size
    if((i*2) == *stsHzLst)
    {
        stsHzLst++;
        *outSat = sumSat;
        outSat++;
        enableStats = 0;
    }
}

UInt16 *hzInterval = (UInt16*)&DisablePaxelSumMacro;

void svuCombDecimDemosaicAwbGainsStats(SippFilter *fptr)
{

    UInt8 *output[3];
    UInt32 *statsOutput;
    UInt16 *iline[4];
    UInt32 clearBuffer = 0;
    CombDecimStatsGainsParam* params = (CombDecimStatsGainsParam*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi         = sippFilterGetLinesPerIter (fptr);
    UInt32 localExeNo  = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, localExeNo++)
    {

        if(0 == localExeNo)
        {
            iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
            iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
            iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
            iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 3, 0);
        }
        else
        {
            if((fptr->outputH-1) == localExeNo)
            {
                iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
                iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
                iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
                iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
            }
            else
            {
                iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
                iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
                iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
                iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 3, 0);
            }
        }

        output[0] = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        output[1] = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        output[2] = (UInt8 *)getOutPtr(fptr, iterNum, 2);

        statsOutput = (UInt32*)((UInt32)params->statsOutput + (params->satPixelsStats->nPatchesX * 4 * params->crtPaxelLine));
        clearBuffer = 0;

        if ((params->runNr*2) == params->paxelsIntervalsVert[params->crtPaxelLine*2])
        {
            hzInterval = (UInt16*)params->paxelsIntervalsHz;
            clearBuffer = 1;
        }
        params->runNr++;
        combDecimDemosaicFinal(output, statsOutput, iline, fptr->sliceWidth, params->gains, params->satPixelsStats->satThresh, hzInterval, clearBuffer);

        if ((params->runNr*2) == params->paxelsIntervalsVert[params->crtPaxelLine*2+1])
        {
            hzInterval = (UInt16*)&DisablePaxelSumMacro;
            params->crtPaxelLine++;
        }
    }
}
