#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBtoUV420/cvtColorRGBtoUV420.h>

/// cvtColorKernel to conversion RGB to UV420
///@param[in]  inR input R channel
///@param[in]  inG input G channel
///@param[in]  inB input B channel
///@param[out] uOut output U channel
///@param[out] vOut output V channel

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBtoUV420_asm(UInt8** inR, UInt8** inG, UInt8** inB, UInt8** uOut, UInt8** vOut, UInt32 width);
#else
void cvtColorRGBtoUV420        (UInt8** inR, UInt8** inG, UInt8** inB, UInt8** uOut, UInt8** vOut, UInt32 width)
{
    UInt32 i,j;

    UInt8* r = inR[0];
    UInt8* g = inG[0];
    UInt8* b = inB[0];
    UInt8* Uo = uOut[0];
    UInt8* Vo = vOut[0];

    float ua, ub, va, vb, um, vm;
    UInt32 u_idx = 0;
    UInt32 v_idx = 0;

    for (i = 0; i < width; i+=2)
    {
        ua = (((float)r[i] * (-0.14713f)) + ((float)g[i] * (-0.28886f)) + ((float)b[i] * 0.436f     ) + 128);
        va = (((float)r[i] * 0.615f     ) + ((float)g[i] * (-0.51499f)) + ((float)b[i] * (-0.10001f)) + 128);

        //-----------------------------------------------------------------------------------------------------------------
        ub = (((float)r[i + 1] * (-0.14713f)) + ((float)g[i + 1] * (-0.28886f)) + ((float)b[i + 1] * 0.436f     ) + 128);
        vb = (((float)r[i + 1] * 0.615f     ) + ((float)g[i + 1] * (-0.51499f)) + ((float)b[i + 1] * (-0.10001f)) + 128);


        um = (ua + ub)/2;
        vm = (va + vb)/2;
        Uo[u_idx] = (UInt8) (um > 255 ? 255 : um < 0 ? 0 : um);
        Vo[v_idx] = (UInt8) (vm > 255 ? 255 : vm < 0 ? 0 : vm);
        u_idx++;
        v_idx++;
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBtoUV420(SippFilter *fptr)
{
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;

    UInt8 *outputU;
    UInt8 *outputV;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        outputU = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outputV = (UInt8 *)getOutPtr(fptr, iterNum, 1);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBtoUV420_asm(&inputR, &inputG, &inputB, &outputU, &outputV, fptr->sliceWidth*2);
    #else
        cvtColorRGBtoUV420(&inputR, &inputG, &inputB, &outputU, &outputV, fptr->sliceWidth*2);
    #endif
    }
}
