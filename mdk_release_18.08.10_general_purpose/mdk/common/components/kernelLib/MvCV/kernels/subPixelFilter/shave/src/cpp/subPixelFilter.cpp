#include "subPixelFilter.h"
#include "stdio.h"


u16
normal4(u8 pre, u8 post)
{
    u8 onion = pre | post;
    int i;

    u16 index;

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


u16 subpixel(u8 preCost, u8 minCost, u8 postCost, u8 minDisparity, u8 numFractionalBit, const u8 maxDisp, volatile u8 * lutToUse)
{
    u16 result;
    u8 pre, post;
    char fractionalOffset;
    
    pre = preCost - minCost;
    post = postCost - minCost;
    result = minDisparity << numFractionalBit;

    //Default LUT: all zeros
    if (!lutToUse)
        return result;


    u16 norm = normal4(pre, post);
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
void mvcvSubPixelFilter(u8* inputDisparityMap, u8* inputCostVolume, u16* outputDisparityMap, u32 width, u8 maxDisp, u8 numFractionalBit, u8 * lutToUse)
{
    u16 j;
    u8 preCost, postCost, minCost;
    u32 postCostIndex, preCostIndex, minCostIndex, lowerBound, upperBound;
    u8 currentDisp;

    for (j = 0; j < width; j++){
        lowerBound = maxDisp*j;
        upperBound = maxDisp*(j+1);
        currentDisp = inputDisparityMap[j];


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
