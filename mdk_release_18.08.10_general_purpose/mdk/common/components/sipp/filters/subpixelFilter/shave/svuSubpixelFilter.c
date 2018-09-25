#include <sipp.h>
#include <sippInternal.h>
#include <sippShaveMacros.h>
#include <filters/subpixelFilter/subpixelFilter.h>

#ifdef __cplusplus
extern "C" {
#endif

void mvcvSubPixelFilter_asm(u8* inputDisparityMap, u8* inputCostVolume, u16* outputDisparityMap, u32 width, u8 maxDisp, u8 numFractionalBit, u8 * lutToUse);

#if 0
UInt16
normal4(UInt8 pre, UInt8 post)
{
    UInt8 onion = pre | post;
    int i;

    UInt16 index;

    if (onion & ~0x3f) { // has a one above six bits, shift down (probably should round here...)
        for (i = 2; i > 0; i--) {// what is the maximum here
            if (onion & (0x20 << i))
                break;
        }
        pre >>= i;
        post >>= i;
    }

    index = (pre << 6) | post;

    return index;
}


UInt16 subpixel(UInt8 preCost, UInt8 minCost, UInt8 postCost, UInt8 minDisparity, UInt8 numFractionalBit, const UInt8 maxDisp, UInt8 * lutToUse)
{
    UInt16 result;
    UInt8 pre, post;
    Int8 fractionalOffset;
    
    pre = preCost - minCost;
    post = postCost - minCost;
    result = minDisparity << numFractionalBit;

    //Default LUT: all zeros
    if (!lutToUse)
        return result;

    UInt16 norm = normal4(pre, post);
    fractionalOffset = lutToUse[norm];

    if (pre < post) {
        fractionalOffset = -fractionalOffset;
    }
    if (minDisparity == maxDisp-1 || minDisparity == 0)
        fractionalOffset = 0;

    result += fractionalOffset;

    return result;
}


//Remember that width is the linked to the output, not the input
void SubpixelFilter (UInt8* inputDisparityMap, UInt8* inputCostVolume, UInt16* outputDisparityMap, UInt32 width, const UInt8 maxDisp, const UInt8 numFractionalBit, UInt8 * lutToUse)
{
    UInt16 j;
    UInt8 preCost, postCost, minCost;
    UInt32 postCostIndex, preCostIndex, minCostIndex, lowerBound, upperBound;
    UInt8 currentDisp;

    for (j = 0; j < width; j++){
        lowerBound = maxDisp*j;
        upperBound = maxDisp*(j+1);
        currentDisp = inputDisparityMap[j];
        //printf("CurrentDisp: %u\n", currentDisp);

        minCostIndex = lowerBound + currentDisp;
        minCost = inputCostVolume[minCostIndex];

        (minCostIndex == 0) ? (preCostIndex = minCostIndex) : (preCostIndex = minCostIndex - 1);
        if(preCostIndex < lowerBound)
            preCostIndex = minCostIndex;
        preCost = inputCostVolume[preCostIndex];

        postCostIndex = minCostIndex + 1;
        if(postCostIndex >= upperBound)
            postCostIndex = minCostIndex;
        postCost = inputCostVolume[postCostIndex];

        outputDisparityMap[j] = subpixel(preCost, minCost, postCost, currentDisp, numFractionalBit, maxDisp, lutToUse);
    }
}
#endif
void svuSubpixelFilter(SippFilter *fptr)
{
	UInt16 *output;
    UInt8 *iline0;
    UInt8 *iline1;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter(fptr);
    UInt32 iterNum;

    SubpixelFilterParam *param = (SubpixelFilterParam*)fptr->params;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        iline0 = (UInt8 *) getInPtr(fptr, 0, iterNum, 0, 0);
        iline1 = (UInt8 *) getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (UInt16 *)getOutPtr(fptr, iterNum, 0);

        mvcvSubPixelFilter_asm(iline0, iline1, output, fptr->sliceWidth, param->numDisp, param->numFractionalBits, (uint8_t*)param->LUT);
        //SubpixelFilter(iline0, iline1, output, fptr->sliceWidth, param->numDisp, param->numFractionalBits, param->LUT);
    }
}
#ifdef __cplusplus
}
#endif
