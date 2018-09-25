#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/whiteBalanceRGB/whiteBalanceRGB.h>

/// White Balance Gains for RGB input
/// @param[in]  inR      - pointer to the first line from input R plane
/// @param[in]  inG      - pointer to the first line from input G plane
/// @param[in]  inB      - pointer to the first line from input B plane
/// @param[Out] outR     - pointer to the first line from output R plane
/// @param[Out] outG     - pointer to the first line from output G plane
/// @param[Out] outB     - pointer to the first line from output B plane
/// @param[in]  awbCoefs - awb gains coefs
/// @param[in]  clamp    - clamp value
/// @param[in]  width    - line width in pixels
/// @return     Nothing

#ifdef SIPP_USE_MVCV
void mvcvWhiteBalanceRGB_asm(UInt16** inR, UInt16** inG, UInt16** inB, UInt16 awbCoef[3], UInt16** outR, UInt16** outG, UInt16** outB, UInt16 clamp[1], UInt32 width);
#else
void whiteBalanceRGB        (UInt16** inR, UInt16** inG, UInt16** inB, UInt16 awbCoef[3], UInt16** outR, UInt16** outG, UInt16** outB, UInt16 clamp[1], UInt32 width)
{
    int i;
    UInt16* rIn = inR[0];
    UInt16* gIn = inG[0];
    UInt16* bIn = inB[0];

    UInt16* rOut = outR[0];
    UInt16* gOut = outG[0];
    UInt16* bOut = outB[0];

    UInt32 r,g,b;

    for (i = 0; i < width; i++)
    {
        r = ((UInt32)rIn[i] * (UInt32)awbCoef[0]) >> 15;
        g = ((UInt32)gIn[i] * (UInt32)awbCoef[1]) >> 15;
        b = ((UInt32)bIn[i] * (UInt32)awbCoef[2]) >> 15;


        rOut[i] = (UInt16)(r > clamp[0] ? clamp[0] : r < 0 ? 0 : r);
        gOut[i] = (UInt16)(g > clamp[0] ? clamp[0] : g < 0 ? 0 : g);
        bOut[i] = (UInt16)(b > clamp[0] ? clamp[0] : b < 0 ? 0 : b);
    }
}
#endif

//#######################################################################################
void svuWhiteBalanceRGB(SippFilter *fptr)
{
    UInt16 *inputR;
    UInt16 *inputG;
    UInt16 *inputB;

    UInt16 *outputR;
    UInt16 *outputG;
    UInt16 *outputB;

    WhiteBalanceRGBParam *param = (WhiteBalanceRGBParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt16*)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt16*)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt16*)getInPtr(fptr, 0, iterNum, 0, 2);

        //the 3 output lines
        outputR = (UInt16*)getOutPtr(fptr, iterNum, 0);
        outputG = (UInt16*)getOutPtr(fptr, iterNum, 1);
        outputB = (UInt16*)getOutPtr(fptr, iterNum, 2);

    #ifdef SIPP_USE_MVCV
        mvcvWhiteBalanceRGB_asm(&inputR, &inputG, &inputB, param->awbCoef, &outputR, &outputG, &outputB, param->clamp, fptr->sliceWidth);
    #else
        whiteBalanceRGB(&inputR, &inputG, &inputB, param->awbCoef, &outputR, &outputG, &outputB, param->clamp, fptr->sliceWidth);
    #endif
    }
}
