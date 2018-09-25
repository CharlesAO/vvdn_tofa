#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBtoUV/cvtColorRGBtoUV.h>

/// cvtColorKernel to conversion RGB to UV
///@param[in]  inR input R channel
///@param[in]  inG input G channel
///@param[in]  inB input B channel
///@param[out] uvOut output UV channels

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBtoUV_asm(UInt8** inR, UInt8** inG, UInt8** inB, UInt8** uvOut, UInt32 width, UInt32 line);
#else
void cvtColorRGBtoUV        (UInt8** inR, UInt8** inG, UInt8** inB, UInt8** uvOut, UInt32 width, UInt32 line)
{
    UInt32 i,j;

    UInt8* r = inR[0];
    UInt8* g = inG[0];
    UInt8* b = inB[0];
    UInt8* uvo = uvOut[0];

    int y,u1, u2, v1, v2, um, vm;
    UInt32 uv_idx = 0;

    for (i = 0; i < width; i+=2)
    {
        // line = i;
        y = 0.299f * r[i] + 0.587f * g[i] + 0.114f * b[i];
        //yo[i] = (UInt8) y > 255 ? 255 : y < 0 ? 0 : y;

        if (line % 2 == 0) {
            u1 = (int)(((float)b[i] - y) * 0.564f) + 128;
            v1 = (int)(((float)r[i] - y) * 0.713f) + 128;
        }
        //-------------------------------------------------------

        y = 0.299f * r[i+1] + 0.587f * g[i+1] + 0.114f * b[i+1];
        //yo[i + 1] = (UInt8) y > 255 ? 255 : y < 0 ? 0 : y;

        if (line % 2 == 0)
        {
            u2 = (int)(((float)b[i+1] - y) * 0.564f) + 128;
            v2 = (int)(((float)r[i+1] - y) * 0.713f) + 128;

            um = (u1 + u2)/2;
            vm = (v1 + v2)/2;
            uvo[uv_idx] = (UInt8) um > 255 ? 255 : um < 0 ? 0 : um;
            uvo[uv_idx + 1] = (UInt8) vm > 255 ? 255 : vm < 0 ? 0 : vm;
            uv_idx = uv_idx + 2;
        }
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBtoUV(SippFilter *fptr)
{
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;
    UInt8 *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBtoUV_asm(&inputR, &inputG, &inputB, &output, fptr->sliceWidth, fptr->exeNo);
    #else
        cvtColorRGBtoUV(&inputR, &inputG, &inputB, &output, fptr->sliceWidth, fptr->exeNo);
    #endif
    }
}
