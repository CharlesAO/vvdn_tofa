#include "censusMin7.h"

void mvcvCensusMin7(u8 *in, u8 *out, u32 width)
{
    u32 disparities = 7;
    for (u32 i = 0; i < width; i++)
    { // for all pixels

        u32 minCost = in[i * disparities];
        u8 minPos = 0;

        for (u32 disp = 1; disp < disparities; disp++)
        { // for all disparities

            if (in[i * disparities + disp] < minCost)
            { // new min
                minCost = in[i * disparities + disp];
                minPos = (u8)(disp);
            }
        }

        // Store the position of min
        out[i] = minPos;
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvCensusMin7_opt(unsigned char * __restrict in, unsigned char * __restrict out, unsigned int width) {
  // Local buffers
  unsigned int inLocal1[DISPARITIES];
  unsigned int inLocal2[DISPARITIES];

  // Pre-load local buffer 1
  for (unsigned int j = 0; j < DISPARITIES; ++j)
    inLocal1[j] = in[j];

  // Pre-load local buffer 2
  for (unsigned int j = 0; j < DISPARITIES; ++j)
    inLocal2[j] = in[DISPARITIES+j];

  #pragma unroll 8
  for ( int i = 0; i < (int)width/2; ++i) {
    //Calculate the result for local buffer 1
    unsigned char minCost1 = inLocal1[0];
    unsigned char minPos1 = 0;

    for (unsigned int disp = 1; disp < DISPARITIES; ++disp) {
      if ((unsigned char) inLocal1[disp] < minCost1) {
        minCost1 = inLocal1[disp];
        minPos1 = (unsigned char)__builtin_shave_iau_addsu_ri(disp, 0);
      }
    }

    // Calculate the result for local buffer 2
    unsigned char minCost2 = inLocal2[0];
    unsigned char minPos2 = 0;

    for (unsigned int disp = 1; disp < DISPARITIES; ++disp) {
      if ((unsigned char) inLocal2[disp] < minCost2) {
        minCost2 = inLocal2[disp];
        minPos2 = (unsigned char)__builtin_shave_iau_addsu_ri(disp, 0);
      }
    }

    unsigned int index = i*2;

    // Pre-load local buffer 1 for the next iteration
    for (unsigned int j = 0; j < DISPARITIES; ++j)
      inLocal1[j] = in[(index+2)*DISPARITIES + j];

    // Pre-load local buffer 2 for the next iteration
    for (unsigned int j = 0; j < DISPARITIES; ++j)
      inLocal2[j] = in[(index+3)*DISPARITIES + j];

    // Store the results
    out[index] = minPos1;
    out[index+1] = minPos2;
  }
}
#endif
