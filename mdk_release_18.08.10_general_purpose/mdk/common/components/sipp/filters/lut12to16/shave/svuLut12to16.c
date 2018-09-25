#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/lut12to16/lut12to16.h>

///Performs a look-up table transform of a line. The function fills the destination line with values from the look-up table. Indices of the entries are taken from the source line
///@param[in]  src    - Pointer to input line
///@param[out] dest   - Pointer to output line
///@param[in]  lut    - Look-up table of 256 elements; should have the same depth as the input line.
/// In the case of multi-channel source and destination lines, the table should either have a single-channel
///(in this case the same table is used for all channels) or the same number of channels as the source/destination line.
///@param[in]  width  - width of input line
///@param[in]  height - the number of lines (defaulted to one line)

#ifdef SIPP_USE_MVCV
void mvcvLUT12to16_asm(UInt16** src, UInt16** dest, const UInt16* lut, UInt32 width, UInt32 height);
#else
void LUT12to16        (UInt16** src, UInt16** dest, const UInt16* lut, UInt32 width, UInt32 height)
{
    int i, j;
    UInt16* in_line = *src;
    UInt16* out_line = *dest;

    for (j = 0; j < width; j++)
    {
        out_line[j] = lut[in_line[j]&0x0FFF];
    }
}
#endif

void svuLut12to16(SippFilter *fptr)
{
    UInt16 *output;
    UInt16 *iline[1];
    Lut12to16Param *param = (Lut12to16Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt16 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvLUT12to16_asm(iline, &output, param->lutValue, fptr->sliceWidth, 1);
    #else
        LUT12to16(iline, &output, param->lutValue, fptr->sliceWidth, 1);
    #endif
    }
}
