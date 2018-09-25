#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMin7/censusMin7.h>

/// mvcvCensusMin - computes minimum of 7 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing
#ifdef SIPP_USE_MVCV
void mvcvCensusMin7_asm(UInt8 *in, UInt8 *out, UInt32 width);
#else
void mvcvCensusMin7    (UInt8 *in, UInt8 *out, UInt32 width)
{
    UInt32 disparities = 7;
    for (UInt32 i = 0; i < width; i++)
    { // for all pixels

        UInt32 minCost = in[i * disparities];
        UInt8 minPos = 0;

        for (UInt32 disp = 1; disp < disparities; disp++)
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

void svuCensusMin7(SippFilter *fptr)
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
        mvcvCensusMin7_asm(input, output, fptr->sliceWidth);
    #else
        mvcvCensusMin7(input, output, fptr->sliceWidth);
    #endif
    }
}
