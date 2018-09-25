#include <sipp.h>
#include <sippShaveMacros.h>

//##########################################################################################
void svuCopy(SippFilter *fptr)
{
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        UInt8 *iline  = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 0); //the input line
        UInt8 *output = (UInt8*)getOutPtr(fptr, iterNum, 0);      //the output line
        UInt32 i;

        //The actual copy
        for(i=0; i<fptr->sliceWidth; i++)
            output[i] = iline[i];
    }
}
