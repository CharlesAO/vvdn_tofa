#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/scale2xBilinHV_U8ToU8_phase025_075/scale2xBilinHV_U8ToU8_phase025_075.h>

/// mvispScale2xBilinHV_025_075_U8ToU8 Upscale by 2
///@param[in] in         - Input lines
///@param[out] out       - Output line
///@param[in] inWidth    - Width of output line

 #ifdef SIPP_USE_MVCV
void mvispScale2xBilinHV025_U8ToU8_asm(UInt8** in, UInt8** out, UInt32 inWidth);
void mvispScale2xBilinHV075_U8ToU8_asm(UInt8** in, UInt8** out, UInt32 inWidth);
#else
void scale2xBilinHV025_U8ToU8         (UInt8** in, UInt8** out, UInt32 inWidth)
{
    UInt8 *output = out[0];
    int j;

    float weight[2][4] = {
            {1.0/16, 3.0/16, 3.0/16, 9.0/16},
            {3.0/16, 1.0/16, 9.0/16, 3.0/16},
    };

    for (j = 0; j < inWidth; j+=2)
    {
        output[j] = (UInt8)((float)in[0][j/2 - 1]*weight[0][0] + (float)in[0][j/2]*weight[0][1] + (float)in[1][j/2-1]*weight[0][2] + (float)in[1][j/2]*weight[0][3]);
        output[j+1] = (UInt8)((float)in[0][j/2]*weight[1][0] + (float)in[0][j/2+1]*weight[1][1] + (float)in[1][j/2]*weight[1][2] + (float)in[1][j/2+1]*weight[1][3]);
    }
}

void scale2xBilinHV075_U8ToU8(UInt8** in, UInt8** out, UInt32 inWidth)
{
    UInt8 *output = out[0];
    int j;

    float weight[2][4] = {
            {3.0/16, 9.0/16, 1.0/16, 3.0/16},
            {9.0/16, 3.0/16, 3.0/16, 1.0/16},
    };

    for (j = 0; j < inWidth; j+=2)
    {
        output[j] = (UInt8)((float)in[0][j/2 - 1]*weight[0][0]+ (float)in[0][j/2]*weight[0][1] + (float)in[1][j/2-1]*weight[0][2]+ (float)in[1][j/2]*weight[0][3]);
        output[j+1] = (UInt8)((float)in[0][j/2]*weight[1][0]+ (float)in[0][j/2+1]*weight[1][1] + (float)in[1][j/2]*weight[1][2]+ (float)in[1][j/2+1]*weight[1][3]);
    }
}
#endif

void svuScale2xBilinHV_U8ToU8_phase025_075(SippFilter *fptr)
{
    UInt32  p;
    UInt8 *in[2];
    UInt8 *out;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr,0);
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++,runNo++)
    {
        for(p=0; p<numPlanes; p++)
        {
            out = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, p/*plane*/, in);
            if((runNo & 1) == 0)
            {
    #ifdef SIPP_USE_MVCV
                mvispScale2xBilinHV025_U8ToU8_asm(in, &out, fptr->sliceWidth);
    #else
                scale2xBilinHV025_U8ToU8(in, &out, fptr->sliceWidth);
    #endif
            }
            else
            {
    #ifdef SIPP_USE_MVCV
                mvispScale2xBilinHV075_U8ToU8_asm(in, &out, fptr->sliceWidth);
    #else
                scale2xBilinHV075_U8ToU8(in, &out, fptr->sliceWidth);
    #endif
            }
        }
    }
}
