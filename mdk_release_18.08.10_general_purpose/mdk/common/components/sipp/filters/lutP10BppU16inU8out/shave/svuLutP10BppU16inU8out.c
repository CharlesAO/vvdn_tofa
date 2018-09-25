#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/lutP10BppU16inU8out/lutP10BppU16inU8out.h>

#define MAX_LUT_IDX ((1<<10) - 1)

//##########################################################################################
void svuLutP10BppU16inU8out(SippFilter *fptr)
{
    UInt32 x;
    YDnsRefLut10bppParam *param = (YDnsRefLut10bppParam *)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Input Y line
        UInt16 *inY  = (UInt16*)getInPtr(fptr, 0, iterNum, 0, 0);

        //Output Y ref
        UInt8 *outY = (UInt8*)getOutPtr(fptr, iterNum, 0);

        for(x=0; x<fptr->sliceWidth; x++)
        {
            outY[x] = param->lut[(inY[x] > MAX_LUT_IDX ? MAX_LUT_IDX : inY[x])];
        }
    }
}
