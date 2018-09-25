#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/pixelUnpacker/pixelUnpacker.h>

/// pixelUnpacker 10b -> 16b/15b/14b/13b/12b/11b/10b depending on the shift value
/// @param[in]  ms8b        - array of pointers to the 8 MSBs
/// @param[in]  ms2b        - array of pointers to the 2 LSBs
/// @param[out] output      - array of pointers for output line
/// @param[in]  width       - width of input line
/// @param[in] shift        - number of bits for shifting the result to left

#ifdef SIPP_USE_MVCV
void mvispPixelUnpacker_asm(UInt32 *ls8b, UInt8 *ms2b, UInt16 *output, UInt32 width, UInt8 shift);
#else
void pixelUnpacker         (UInt32 *ls8b, UInt8 *ms2b, UInt16 *output, UInt32 width, UInt8 shift)
{
    UInt8 *ls8b_in = (UInt8*)ls8b;
    UInt8 *out = (UInt8*)output;
    UInt16 *out_shift = (UInt16*)output;
    int ls8b_count = 0;
    int ms2b_count = 0;
    UInt8 ms2b_1, ms2b_2, ms2b_3, ms2b_4, ms2b_5, ms2b_6, ms2b_7, ms2b_8;
    UInt8 mask1 = 0xC0;
    UInt8 mask2 = 0x30;
    UInt8 mask3 = 0x0C;
    UInt8 mask4 = 0x03;

    for(int i = 0; i < (int)width * 2; i = i + 16)
    {
        out[i + 0]  = ls8b_in[ls8b_count++];
        out[i + 2]  = ls8b_in[ls8b_count++];
        out[i + 4]  = ls8b_in[ls8b_count++];
        out[i + 6]  = ls8b_in[ls8b_count++];
        out[i + 8]  = ls8b_in[ls8b_count++];
        out[i + 10] = ls8b_in[ls8b_count++];
        out[i + 12] = ls8b_in[ls8b_count++];
        out[i + 14] = ls8b_in[ls8b_count++];

        ms2b_1 = (ms2b[ms2b_count] & mask1)  >> 6;
        ms2b_2 = (ms2b[ms2b_count] & mask2)  >> 4;
        ms2b_3 = (ms2b[ms2b_count] & mask3)  >> 2;
        ms2b_4 = ms2b[ms2b_count++] & mask4;

        ms2b_5 = (ms2b[ms2b_count] & mask1) >> 6;
        ms2b_6 = (ms2b[ms2b_count] & mask2) >> 4;
        ms2b_7 = (ms2b[ms2b_count] & mask3) >> 2;
        ms2b_8 = ms2b[ms2b_count++] & mask4;

        out[i + 1]  = ms2b_1;
        out[i + 5]  = ms2b_2;
        out[i + 9]  = ms2b_3;
        out[i + 13] = ms2b_4;
        out[i + 3]  = ms2b_5;
        out[i + 7]  = ms2b_6;
        out[i + 11] = ms2b_7;
        out[i + 15] = ms2b_8;
    }

    for(int i = 0; i < (int)width; i++)
        out_shift[i] = out_shift[i] << shift;
}
#endif

void svuPixelUnpacker(SippFilter *fptr)
{
    UInt8 *input8;
    UInt32 *input32;
    UInt16 *output16;
    PixelUnpackerParam *param = (PixelUnpackerParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //input lines
        input32 = (UInt32*)getInPtr(fptr,0,iterNum,0,0);
        input8  = (UInt8*)getInPtr(fptr,1,iterNum,0,0);

        //output line
        output16 = (UInt16*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvispPixelUnpacker_asm(input32, input8, output16, fptr->sliceWidth, param->shift);
    #else
        pixelUnpacker(input32, input8, output16, fptr->sliceWidth, param->shift);
    #endif
    }
}
