#include "hammingDistance.h"
#include <stdlib.h>
#include <bitset>

//countBit() - Compute Hamming distance between two numbers
int countBit(unsigned int resultXOR)
{
   unsigned int i;
   int returnValue = 0;

   for (i = 0; i < resultXOR; resultXOR >>= 1)
   {
      returnValue += (resultXOR & 1);
   }

   return returnValue;
}

// Find matches between two descriptors
void mvcvHammingDistance(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances)
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
#ifdef MOVICOMPILE_OPTIMIZED
static inline void __attribute__((always_inline)) hammingDistance_impl(uchar16 * __restrict d1, unsigned char * __restrict d2, int array_size, int descriptor_size, unsigned short * __restrict distances) {
  for (int i = 0; i < array_size; i+=8) {
    ushort8 sums = {0, 0, 0, 0, 0, 0, 0, 0};
    for(unsigned int k = 0; k < 8; ++k) {
      uchar16 * d2_local = (uchar16 *) &d2[descriptor_size*(i+k)];
      for (int j = 0; j < descriptor_size/16; ++j) {
        sums[k] += __builtin_shave_sau_onesx_x128_r(d1[j] ^ d2_local[j]);
      }
    }
    *((ushort8 *) &distances[i]) = sums;
  }
}

void mvcvHammingDistance_opt(unsigned char * __restrict d1, unsigned char * __restrict d2, int array_size, int descriptor_size, unsigned short * __restrict distances) { 
  uchar16 d1_local[8];
  memcpy(d1_local, d1, 128);
  
  if (descriptor_size == 128)
    hammingDistance_impl(d1_local, d2, array_size, 128, distances);
  else if (descriptor_size == 64)
    hammingDistance_impl(d1_local, d2, array_size, 64, distances);
  else
    hammingDistance_impl(d1_local, d2, array_size, 32, distances);
}
#endif
