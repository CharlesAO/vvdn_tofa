#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/whiteBalanceBayerGBRG/whiteBalanceBayerGBRG.h>

/// White Balance Gains for BayerGBRG input
/// @param[in]  inGBRG     - pointer to the first line from input
/// @param[Out] outGBRG    - pointer to the first line from output R plane
/// @param[in]   awbCoef   - awb gains coefs
/// @param[in]   clamp     - clamp value
/// @param[in]   line      - line's number
/// @param[in]   width     - line width in pixels
/// @return     Nothing

#ifdef SIPP_USE_MVCV
void mvcvWhiteBalanceBayerGBRG_asm(UInt16** inGBRG, UInt16 awbCoef[3], UInt16** outGBRG, UInt16 clamp[1], UInt32 width, UInt32 line);
#else
void whiteBalanceBayerGBRG        (UInt16** inGBRG, UInt16 awbCoef[3], UInt16** outGBRG, UInt16 clamp[1], UInt32 width, UInt32 line)
{
    int i;
    UInt32 o1, o2, o3, o4;

    for (i = 0; i < width; i+=2)
    {
        if (line % 2 == 0)
        {
            o1 = ((UInt32)inGBRG[0][0+i] * (UInt32)awbCoef[1]) >> 15;
            o2 = ((UInt32)inGBRG[0][1+i] * (UInt32)awbCoef[2]) >> 15;
                         outGBRG[0][0+i] = (UInt16)(o1 > clamp[0] ? clamp[0] : o1 < 0 ? 0 : o1);
                         outGBRG[0][1+i] = (UInt16)(o2 > clamp[0] ? clamp[0] : o2 < 0 ? 0 : o2);
        }
        else
        {
            o3 = ((UInt32)inGBRG[0][0+i] * (UInt32)awbCoef[0]) >> 15;
            o4 = ((UInt32)inGBRG[0][1+i] * (UInt32)awbCoef[1]) >> 15;
                         outGBRG[0][0+i] = (UInt16)(o3 > clamp[0] ? clamp[0] : o3 < 0 ? 0 : o3);
                         outGBRG[0][1+i] = (UInt16)(o4 > clamp[0] ? clamp[0] : o4 < 0 ? 0 : o4);
        }
    }
}
#endif

//#######################################################################################
void svuWhiteBalanceBayerGBRG(SippFilter *fptr)
{
    UInt16 *input;
    UInt16 *output;

    WhiteBalanceBayerGBRGParam *param = (WhiteBalanceBayerGBRGParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 lpi              = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo            = fptr->exeNo * lpi;
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //the input line
        input = (UInt16*)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt16*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvWhiteBalanceBayerGBRG_asm(&input, param->awbCoef, &output, param->clamp, fptr->sliceWidth, runNo);
    #else
        whiteBalanceBayerGBRG(&input, param->awbCoef, &output, param->clamp, fptr->sliceWidth, runNo);
    #endif
    }
}