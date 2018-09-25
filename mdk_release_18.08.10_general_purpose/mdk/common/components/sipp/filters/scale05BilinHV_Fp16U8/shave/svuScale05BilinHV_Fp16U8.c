#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/scale05BilinHV_Fp16U8/scale05BilinHV_Fp16U8.h>

//#####################################################################################
/// scale05BilinHV_fp16U8 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] inWidth      - width of input line

#if defined(SIPP_USE_MVCV)
void mvispScale05BilinHV_Fp16U8_asm(half** in, UInt8** out, UInt32 inWidth);
#else
void scale05BilinHV_Fp16U8         (half** in, UInt8** out, UInt32 inWidth)
{
    int i;
    float tmp;

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        tmp = (((float)in[0][i*2] + (float)in[0][i*2+1] +
               (float)in[1][i*2] + (float)in[1][i*2+1]) * (float)(255.0f/4.0f));

        half tmp2 = (half)(tmp);
        if (tmp2 >= 255.0f)
            tmp2 = 255;
        if (tmp2 <= 0.0f)
            tmp2 = 0;
        out[0][i] = (UInt8)(tmp2);
    }
}
#endif

//#####################################################################################
void svuScale05BilinHV_Fp16U8(SippFilter *fptr)
{
    UInt32 pl;
    half  *in[2];
    UInt8 *out;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        // Two input lines needed (bilinear)

        for(pl = 0; pl < numPlanes; pl++)
        {
            in[0] = (half *)getInPtr(fptr, 0, iterNum, 0, pl);
            in[1] = (half *)getInPtr(fptr, 0, iterNum, 1, pl);

            out   = (UInt8 *)getOutPtr(fptr, iterNum, pl);

    #ifdef SIPP_USE_MVCV
            mvispScale05BilinHV_Fp16U8_asm(in, &out, fptr->sliceWidth);
    #else
            scale05BilinHV_Fp16U8         (in, &out, fptr->sliceWidth);
    #endif
        }
    }
}
