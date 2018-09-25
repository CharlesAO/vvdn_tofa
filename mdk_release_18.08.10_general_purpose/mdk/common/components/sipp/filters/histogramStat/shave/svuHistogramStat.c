#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/histogramStat/histogramStat.h>

/// HistogramStats kernel
/// @param[in]  inputR      - pointer to the R component
/// @param[in]  inputG      - pointer to the G component
/// @param[in]  inputB      - pointer to the B component
/// @param[Out] histR       - pointer to histogram for R component
/// @param[Out] histG       - pointer to histogram for G component
/// @param[Out] histB       - pointer to histogram for B component
/// @param[in]  width       - width of the input line
/// @param[in]  step        - step*8 will be the number of skipped pixeles
/// @return     Nothing

#ifdef SIPP_USE_MVCV
void mvispHistogramStat_asm(UInt8 *inputR, UInt8 *inputG, UInt8 *inputB, UInt32 *histR, UInt32 *histG, UInt32 *histB, UInt32 width, UInt32 step);
#else
void mvispHistogramStat     (UInt8 *inputR, UInt8 *inputG, UInt8 *inputB, UInt32 *histR, UInt32 *histG, UInt32 *histB, UInt32 width, UInt32 step)
{
    int incr = step * 8 + 8;

    for(int i = 0; i < width - 7; i+=incr)
    {
        for(int j = 0; j < 8; j++)
        {
            histR[inputR[i+j]>>2]++;
            histG[inputG[i+j]>>2]++;
            histB[inputB[i+j]>>2]++;
        }
    }
}
#endif

//#######################################################################################
void svuHistogramStat(SippFilter *fptr)
{
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;

    UInt32 *outputR;
    UInt32 *outputG;
    UInt32 *outputB;

    HistogramStatParam *param = (HistogramStatParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 2);

        //the 3 output lines
        outputR = (UInt32*)getOutPtr(fptr, iterNum, 0);
        outputG = (UInt32*)getOutPtr(fptr, iterNum, 1);
        outputB = (UInt32*)getOutPtr(fptr, iterNum, 2);

        UInt32 i;
        for(i = 0; i < fptr->sliceWidth; i++)
        {
            outputR[i] = 0;
            outputG[i] = 0;
            outputB[i] = 0;
        }

     #ifdef SIPP_USE_MVCV
        mvispHistogramStat_asm(inputR, inputG, inputB, outputR, outputG, outputB, fptr->sliceWidth, param->step);
     #else
        mvispHistogramStat(inputR, inputG, inputB, outputR, outputG, outputB, fptr->sliceWidth, param->step);
    #endif
    }
}
