#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genLumaU8Fp16/genLumaU8Fp16.h>

//##########################################################################################
#if defined(SIPP_USE_MVCV)
void mvispGenLumaU8Fp16_asm(UInt8* inR, UInt8 *inG, UInt8 *inB, half *outY, half* coefs, int width);
#else
void genLumaU8Fp16         (UInt8* inR, UInt8 *inG, UInt8 *inB, half *outY, half* coefs, int width)
{
    int i;
    for(i=0; i<width; i++)
    {
        outY[i] = (inR[i]*coefs[0] + inG[i]*coefs[1] + inB[i]*coefs[2]) * (half)(1.0f/255.0f);
    }
}
#endif

//##########################################################################################
void svuGenLumaU8Fp16(SippFilter *fptr)
{
    //Input RGB lines
    UInt8 *inRGB[3];

    GenLumaU8Fp16Param *param = (GenLumaU8Fp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Output FP16
        half *outY = (half *)getOutPtr(fptr, iterNum, 0);

        getIn3PlanePtr(fptr, 0, iterNum, 0, inRGB); //get the 3 input ptrs

    #ifdef SIPP_USE_MVCV
        mvispGenLumaU8Fp16_asm(inRGB[0], inRGB[1], inRGB[2], outY, (half*)param->coefs, fptr->sliceWidth);
    #else
        genLumaU8Fp16         (inRGB[0], inRGB[1], inRGB[2], outY, (half*)param->coefs, fptr->sliceWidth);
    #endif
    }
}
