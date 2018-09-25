#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/greyDesat/greyDesat.h>

#if !defined(SIPP_USE_MVCV)
static inline int absGreyDesat(int x)
{
  if(x<0) x = -x;
  return  x;
}
#endif

//###################################################################################
#if defined(SIPP_USE_MVCV)
void mvispGreyDesat_asm(UInt8** in, UInt8** out, Int32 offset, Int32 slope, Int32 grey[3], UInt32 width);
#else
void greyDesat         (UInt8** in, UInt8** out, Int32 offset, Int32 slope, Int32 grey[3], UInt32 width)
{
    UInt32 i;
    short alpha;
    short off = (short)offset;
    short slp = (short)slope;
    short g[3];

    g[0] = (short)grey[0];
    g[1] = (short)grey[1];
    g[2] = (short)grey[2];

    for (i = 0; i < width; i++) {
        alpha = absGreyDesat((int)in[0][i] - g[0]) +
            absGreyDesat((int)in[1][i] - g[1]) +
            absGreyDesat((int)in[2][i] - g[2]);

    	alpha = clampU8(((alpha + off) * slp));

        out[0][i] = (UInt8)(((alpha * (in[0][i] - g[0])) >> 8) + g[0]);
        out[1][i] = (UInt8)(((alpha * (in[1][i] - g[1])) >> 8) + g[1]);
        out[2][i] = (UInt8)(((alpha * (in[2][i] - g[2])) >> 8) + g[2]);
    }
}
#endif

//###################################################################################
void svuGreyDesat(SippFilter *fptr)
{
    UInt8 *in[3], *out[3];

    GreyDesatParam *param = (GreyDesatParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
    #if 0
        in[0]  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        in[1]  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        in[2]  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        out[0] = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        out[1] = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        out[2] = (UInt8 *)getOutPtr(fptr, iterNum, 2);
    #else
        //faster access;
        out[0] = (UInt8 *)getPlaneIoPtrs(fptr, 0, iterNum, 0, &in[0]);
        out[1] = (UInt8 *)getPlaneIoPtrs(fptr, 0, iterNum, 1, &in[1]);
        out[2] = (UInt8 *)getPlaneIoPtrs(fptr, 0, iterNum, 2, &in[2]);
    #endif

    #ifdef SIPP_USE_MVCV
        mvispGreyDesat_asm(in, out, param->offset, param->slope, param->grey, fptr->sliceWidth);
    #else
        greyDesat         (in, out, param->offset, param->slope, param->grey, fptr->sliceWidth);
    #endif
    }
}
