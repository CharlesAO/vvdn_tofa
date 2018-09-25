#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMin16/censusMin16.h>

/// mvcvCensusMin - computes minimum of 16 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing
#ifdef SIPP_USE_MVCV
void mvcvCensusMin16_asm(UInt8 *in, UInt8 *out, UInt32 width);
#else
void mvcvCensusMin16    (UInt8 *in, UInt8 *out, UInt32 width)
{
    u32 disparities = 16;
    for (u32 i = 0; i < width; i++)
    { // for all pixels

        u32 minCost = in[i * disparities];
        UInt8 minPos = 0;

        for (u32 disp = 1; disp < disparities; disp++)
        { // for all disparities

            if (in[i * disparities + disp] < minCost)
            { // new min
                minCost = in[i * disparities + disp];
                minPos = (UInt8)(disp);
            }
        }
        // Store the position of min
        out[i] = minPos;
    }
}

#endif

void svuCensusMin16(SippFilter *fptr)
{
    UInt8 *input;
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCensusMin16_asm(input, output, fptr->sliceWidth);
    #else
        mvcvCensusMin16(input, output, fptr->sliceWidth);
    #endif
    }
}
