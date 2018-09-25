#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/pixelPacker10b/pixelPacker10b.h>

/// pixelPacker 16b -> 10b
/// @param[in]  srcAddr     - array of pointers to input line
/// @param[out] ls8b        - array of pointers for output line of ls8b
/// @param[out] ms2b        - array of pointers for output line of ms2b
/// @param[in]  width       - width of input line

#ifdef SIPP_USE_MVCV
void mvispPixelPacker10b_asm(UInt16* srcAddr, UInt32* ls8b, UInt8 *ms2b, UInt32 width);
#else
void pixelPacker10b         (UInt16* srcAddr, UInt32* ls8b, UInt8 *ms2b, UInt32 width)
{
    int msb_count = 0;
    int lsb_count = 0;
    int mask = 0x0300;
    int mask2 = 0x00ff;
    UInt16 in;
    UInt16 in1_msb, in2_msb, in3_msb, in4_msb, in5_msb, in6_msb, in7_msb, in8_msb;
    UInt32 out1_32, out2_32;
    UInt8 *out8;
    UInt8 *ls8b_out = (UInt8*)ls8b;

    for(int i = 0; i < width; i = i + 8)
    {
        in = srcAddr[i + 0] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 1] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 2] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 3] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 4] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 5] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 6] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in = srcAddr[i + 7] & mask2;
        out8 = (UInt8*)&in;
        ls8b_out[lsb_count++] = *out8;

        in1_msb = (srcAddr[i + 0] & mask) >> 2;
        in2_msb = (srcAddr[i + 1] & mask) >> 2;
        in3_msb = (srcAddr[i + 2] & mask) >> 4;
        in4_msb = (srcAddr[i + 3] & mask) >> 4;
        in5_msb = (srcAddr[i + 4] & mask) >> 6;
        in6_msb = (srcAddr[i + 5] & mask) >> 6;
        in7_msb = (srcAddr[i + 6] & mask) >> 8;
        in8_msb = (srcAddr[i + 7] & mask) >> 8;

        in1_msb = in1_msb | in3_msb | in5_msb | in7_msb;
        in2_msb = in2_msb | in4_msb | in6_msb | in8_msb;

        out8 = (UInt8*)&in1_msb;
        ms2b[msb_count++] = *out8;
        out8 = (UInt8*)&in2_msb;
        ms2b[msb_count++] = *out8;
    }
}
#endif

void svuPixelPacker10b(SippFilter *fptr)
{
    UInt8 *output8;
    UInt32 *output32;
    UInt16 *input;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //input line
        input = (UInt16*)getInPtr(fptr,0,iterNum,0,0);

        //output line (2 planes)
        output32 = (UInt32*)getOutPtr(fptr, iterNum, 0);
        output8  = (UInt8 *)getOutPtr(fptr, iterNum, 1);

    #ifdef SIPP_USE_MVCV
        mvispPixelPacker10b_asm(input, output32, output8, fptr->sliceWidth);
    #else
        pixelPacker10b(input, output32, output8, fptr->sliceWidth);
    #endif
    }
}
