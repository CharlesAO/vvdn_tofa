#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/pixelUnpackerWB/pixelUnpackerWB.h>

//#######################################################################################
/// pixelUnpacker 10b -> 16b/15b/14b/13b/12b/11b/10b depending on the shift value
/// @param[in]  ms8b        - array of pointers to the 8 MSBs
/// @param[in]  ms2b        - array of pointers to the 2 LSBs
/// @param[out] output      - array of pointers for output line
/// @param[in]  width       - width of input line
/// @param[in] shift        - number of bits for shifting the result to left

#if defined(SIPP_USE_MVCV)
extern void mvispPixelUnpackerWB_asm(UInt32 *ms8b,  UInt8  *ls2b, UInt16  *output, UInt32 width, UInt8 shift, UInt16 *awbCoef, UInt32 line);
#else
void pixelUnpackerWB                (UInt32 **ms8b, UInt8 **ls2b, UInt16 **output, UInt32 width, UInt8 shift, UInt16 *awbCoef, UInt32 line)
{
    UInt8 **ms8b_in = (UInt8**)ms8b;
    UInt8 *ls2b_in = *ls2b;
    UInt8 **out = (UInt8**)output;
    UInt16 **out_shift = (UInt16**)output;
    int ms8b_count = 0;
    int ls2b_count = 0;
    UInt8 ls2b_1, ls2b_2, ls2b_3, ls2b_4, ls2b_5, ls2b_6, ls2b_7, ls2b_8;
    UInt8 mask1 = 0xC0;
    UInt8 mask2 = 0x30;
    UInt8 mask3 = 0x0C;
    UInt8 mask4 = 0x03;
    UInt16 clamp = 0x3fff;
    int i;
    int j = 0;
    UInt32 o1, o2, o3, o4;

    /// Unpacker
    for(i = 0; i < width * 2; i = i + 16)
    {
        out[0][i + 0]   = ms8b_in[0][ms8b_count++];
        out[0][i + 2]   = ms8b_in[0][ms8b_count++];
        out[0][i + 4]   = ms8b_in[0][ms8b_count++];
        out[0][i + 6]   = ms8b_in[0][ms8b_count++];
        out[0][i + 8]   = ms8b_in[0][ms8b_count++];
        out[0][i + 10]  = ms8b_in[0][ms8b_count++];
        out[0][i + 12]  = ms8b_in[0][ms8b_count++];
        out[0][i + 14]  = ms8b_in[0][ms8b_count++];

        ls2b_1 = (ls2b_in[ls2b_count] & mask1)  >> 6;
        ls2b_2 = (ls2b_in[ls2b_count] & mask2)  >> 4;
        ls2b_3 = (ls2b_in[ls2b_count] & mask3)  >> 2;
        ls2b_4 = ls2b_in[ls2b_count++] & mask4;

        ls2b_5 = (ls2b_in[ls2b_count] & mask1)  >> 6;
        ls2b_6 = (ls2b_in[ls2b_count] & mask2)  >> 4;
        ls2b_7 = (ls2b_in[ls2b_count] & mask3)  >> 2;
        ls2b_8 = ls2b_in[ls2b_count++] & mask4;

        out[0][i + 1]   = ls2b_1;
        out[0][i + 5]   = ls2b_2;
        out[0][i + 9]   = ls2b_3;
        out[0][i + 13]  = ls2b_4;
        out[0][i + 3]   = ls2b_5;
        out[0][i + 7]   = ls2b_6;
        out[0][i + 11]  = ls2b_7;
        out[0][i + 15]  = ls2b_8;
    }

    j = 0;
    for(i = 0; i < width * 2; i = i + 2)
        out_shift[0][j++] = (out[0][i  + 1] << 8) + out[0][i];

    for(i = 0; i < width ; i++)
        out_shift[0][i] = out_shift[0][i] << shift;

    /// White balance gains
    for (i = 0; i < width; i+=2)
    {
        if (line % 2 == 0)
        {
            o1 = (UInt32)((UInt32)out_shift[0][0+i] * (UInt32)awbCoef[1]) >> 8;
            o2 = (UInt32)((UInt32)out_shift[0][1+i] * (UInt32)awbCoef[2]) >> 8;
            out_shift[0][0+i] = (UInt16)(o1 > clamp ? clamp : o1 < 0 ? 0 : o1);
            out_shift[0][1+i] = (UInt16)(o2 > clamp ? clamp : o2 < 0 ? 0 : o2);
        }
        else
        {
            o3 = (UInt32)((UInt32)out_shift[0][0+i] * (UInt32)awbCoef[0]) >> 8;
            o4 = (UInt32)((UInt32)out_shift[0][1+i] * (UInt32)awbCoef[3]) >> 8;
            out_shift[0][0+i] = (UInt16)(o3 > clamp ? clamp : o3 < 0 ? 0 : o3);
            out_shift[0][1+i] = (UInt16)(o4 > clamp ? clamp : o4 < 0 ? 0 : o4);
        }
    }
}
#endif

//#######################################################################################
void svuPixelUnpackerWB(SippFilter *fptr)
{
    UInt8 *input8;
    UInt32 *input32;
    UInt16 *output16;
    PixelUnpackerWBParam *param = (PixelUnpackerWBParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi   = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //input lines
        input32 = (UInt32*)getInPtr(fptr,0,iterNum,0,0);
        input8  = (UInt8 *)getInPtr(fptr,1,iterNum,0,0);

        //output line
        output16 = (UInt16*)getOutPtr(fptr, iterNum,0);

    #ifdef SIPP_USE_MVCV
        mvispPixelUnpackerWB_asm(input32,  input8,   output16, fptr->sliceWidth, param->shift, param->awbCoef, runNo);
    #else
        pixelUnpackerWB         (&input32, &input8, &output16, fptr->sliceWidth, param->shift, param->awbCoef, runNo);
    #endif
    }
}

