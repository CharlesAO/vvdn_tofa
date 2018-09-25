#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/hammingDistance/hammingDistance.h>

/// This kernel find matches between two descriptors
/// @param[in] d1              - First descriptor
/// @param[in] d2              - Second descriptor
/// @param[in] array_size      - Size of array used to compute the distances
/// @param[in] descriptor_size - Size of the descriptor (32, 64 or 128 bytes)
/// @param[out] out            - Pointer to distances
/// @return    Nothing

#ifdef SIPP_USE_MVCV
void mvcvHammingDistance_asm(UInt8 *d1, UInt8 *d2, UInt32 array_size, UInt32 descriptor_size, UInt16 *distances);
#else
//countBit() - Compute Hamming distance between two numbers
int countBit(unsigned int resultXOR)
{
    int i;
    int returnValue = 0;

    for (i = 0; i < resultXOR; resultXOR >>= 1)
    {
        returnValue += (resultXOR & 1);
    }

   return returnValue;
}

// Find matches between two descriptors
void mvcvHammingDistance(UInt8 *d1, UInt8 *d2, UInt32 array_size, UInt32 descriptor_size, UInt16 *distances)
{
   unsigned int d = 0, resultXOR = 0;
   int          k, tIdx;

   for(tIdx = 0; tIdx < array_size; tIdx++)
   {
        // compute Hamming distance between a single keypoint descriptor and all descriptors from the given array
        d = 0;
        for (k = 0; k < descriptor_size; k++)
        {
            resultXOR = d1[k] ^ d2[k];
            d += countBit(resultXOR);
        }
        // save error in distances array
        distances[tIdx] = d;
        // go to next keypoint descriptor from the array
        d2 += descriptor_size;
   }
}
#endif

void svuHammingDistance(SippFilter *fptr)
{
    UInt8 *d1;
    UInt8 *d2;
    UInt16 *distances;
    HammingDistanceParam *param = (HammingDistanceParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input lines
        d1 = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        d2 = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        distances = (UInt16*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvHammingDistance_asm(d1, d2, fptr->sliceWidth, param->descriptor_size, distances);
    #else
        mvcvHammingDistance(d1, d2, fptr->sliceWidth, param->descriptor_size, distances);
    #endif
    }
}
