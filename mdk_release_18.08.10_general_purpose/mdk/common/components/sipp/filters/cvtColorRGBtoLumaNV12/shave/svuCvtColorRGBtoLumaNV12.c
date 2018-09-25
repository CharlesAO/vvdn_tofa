#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBtoLumaNV12/cvtColorRGBtoLumaNV12.h>

/// cvtColorKernel to conversion RGB to Luma
///@param[in] inR input R channel
///@param[in] inG input G channel
///@param[in] inB input B channel
///@param[out] yOut output Y channel

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBtoLumaNV12_asm(UInt8* inR, UInt8* inG, UInt8* inB, UInt8* yOut, float* coefsMat, float* offset, UInt32 width);
#else
void cvtColorRGBtoLumaNV12        (UInt8* inR, UInt8* inG, UInt8* inB, UInt8* yOut, float* coefsMat, float* offset, UInt32 width)
{
    UInt32 i;
    //coefsMat = {0.299f, 0.587f, 0.114f};
    float y,r,g,b;

    for (i = 0; i < width; i++)
    {
        r = (float)inR[i]/255;
        g = (float)inG[i]/255;
        b = (float)inB[i]/255;

        y = (coefsMat[0] * r + coefsMat[1] * g + coefsMat[2] * b) + offset[0];
        y = y * 255;

        yOut[i] = (UInt8) ((y > 255 ? 255 : y < 0 ? 0 : y));
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBtoLumaNV12(SippFilter *fptr)
{
    UInt8 *inputRGB[3];
    UInt8 *output;
    cvtColorLumaNV12Param *param = (cvtColorLumaNV12Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the 3 input lines
        getIn3PlanePtr(fptr, 0, iterNum, 0, inputRGB);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBtoLumaNV12_asm(inputRGB[0], inputRGB[1], inputRGB[2], output, param->coefsMat, param->offset, fptr->sliceWidth);
    #else
        cvtColorRGBtoLumaNV12        (inputRGB[0], inputRGB[1], inputRGB[2], output, param->coefsMat, param->offset, fptr->sliceWidth);
    #endif
    }
}
