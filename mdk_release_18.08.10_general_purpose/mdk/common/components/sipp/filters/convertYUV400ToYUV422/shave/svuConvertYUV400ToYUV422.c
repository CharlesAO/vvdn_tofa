#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convertYUV400ToYUV422/convertYUV400ToYUV422.h>

//##########################################################################################
/// ConvertYUV400ToYUV422
/// @param[in] out        - pointer to output line
/// @param[in] in         - pointer to input line
/// @param[in] width      - width of input line

#if defined(SIPP_USE_MVCV)
void mvcvConvertYUV400ToYUV422_asm(UInt8* in, UInt16* out, UInt32 width);
#else
void ConvertYUV400ToYUV422        (UInt8* in, UInt16* out, UInt32 width)
{
    UInt8* inLine = in;
    UInt16* outLine = out;

    for (u32 i = 0; i < width; i++)
    {
        outLine[i] = inLine[i] | 0x8000;
    }
}
#endif

//##########################################################################################
void svuConvertYUV400ToYUV422(SippFilter *fptr)
{
    UInt16 *output;
    UInt8 *iline;
    UInt32 pl;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for (pl = 0; pl < numPlanes; pl++)
        {
            //the input line
            iline=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, pl);

            //the output line
            output = (UInt16 *)getOutPtr(fptr, iterNum, pl);

    #ifdef SIPP_USE_MVCV
            mvcvConvertYUV400ToYUV422_asm(iline, output, fptr->sliceWidth);
    #else
            ConvertYUV400ToYUV422        (iline, output, fptr->sliceWidth);
    #endif
        }
    }
}
