#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/scale2xBilinHV_025_075_Fp16ToFp16/scale2xBilinHV_025_075_Fp16ToFp16.h>

/// mvispScale2xBilinHV_025_075_Fp16ToFp16 Upscale by 2
///@param[in] in         - Input lines
///@param[out] out       - Output line
///@param[in] inWidth    - Width of output line

#ifdef SIPP_USE_MVCV
void mvispScale2xBilinHV025_Fp16ToFp16_asm(half** in, half** out, UInt32 inWidth);
void mvispScale2xBilinHV075_Fp16ToFp16_asm(half** in, half** out, UInt32 inWidth);
#else
void scale2xBilinHV025_Fp16ToFp16         (half** in, half** out, UInt32 inWidth)
{
    half *output = out[0];
    int j;

    half weight[2][4] = {
            {1.0/16, 3.0/16, 3.0/16, 9.0/16},
            {3.0/16, 1.0/16, 9.0/16, 3.0/16},
    };

    for (j = 0; j < inWidth; j+=2)
    {
        output[j] = in[0][j/2 - 1] * weight[0][0] + in[0][j/2]   * weight[0][1]   + in[1][j/2-1] * weight[0][2] + in[1][j/2]   * weight[0][3];
        output[j+1] = in[0][j/2]   * weight[1][0] + in[0][j/2+1] * weight[1][1]   + in[1][j/2]   * weight[1][2] + in[1][j/2+1] * weight[1][3];
    }
}

void scale2xBilinHV075_Fp16ToFp16(half** in, half** out, UInt32 inWidth)
{
    half *output = out[0];
    int j;

    half weight[2][4] = {
      {3.0/16, 9.0/16, 1.0/16, 3.0/16},
      {9.0/16, 3.0/16, 3.0/16, 1.0/16},
    };

    for (j = 0; j < inWidth; j+=2)
    {
    output[j] = in[0][j/2 - 1] * weight[0][0] + in[0][j/2]   * weight[0][1]   + in[1][j/2-1] * weight[0][2] + in[1][j/2]   * weight[0][3];
        output[j+1] = in[0][j/2]   * weight[1][0] + in[0][j/2+1] * weight[1][1]   + in[1][j/2]   * weight[1][2] + in[1][j/2+1] * weight[1][3];
    }
}
#endif

void svuScale2xBilinHV_025_075_Fp16ToFp16(SippFilter *fptr)
{
    UInt32  p;
    half *in[2];
    half *out;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr,0);
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++,runNo++)
    {
        for(p=0; p<numPlanes; p++)
        {
            out = (half*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, p/*plane*/, in);
            if((runNo & 1) == 0)
            {
    #ifdef SIPP_USE_MVCV
                mvispScale2xBilinHV025_Fp16ToFp16_asm(in, &out, fptr->sliceWidth);
    #else
                scale2xBilinHV025_Fp16ToFp16(in, &out, fptr->sliceWidth);
    #endif
            }
            else
            {
    #ifdef SIPP_USE_MVCV
                mvispScale2xBilinHV075_Fp16ToFp16_asm(in, &out, fptr->sliceWidth);
    #else
                scale2xBilinHV075_Fp16ToFp16(in, &out, fptr->sliceWidth);
    #endif
            }
        }
    }
}
