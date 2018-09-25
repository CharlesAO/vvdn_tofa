#include "censusMin16.h"


void mvcvCensusMin16(u8 *in, u8 *out, u32 width)
{
	u32 disparities = 16;
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
void mvcvCensusMin16_opt(unsigned char * __restrict in, unsigned char * __restrict out, unsigned int width) {
  const unsigned int disparities = 16;
  unsigned int local[disparities];
  
  for (unsigned int i = 0; i < disparities; ++i)
    local[i] = in[i];
  
	for (unsigned int i = 0; i < width; ++i) {
    unsigned int minCost = local[0];
    unsigned char minPos = 0;
    
    for (unsigned int disp = 1; disp < disparities; ++disp) {
      if (local[disp] < minCost) {
        minCost = local[disp];
        minPos = (unsigned char) disp;
      }
    }
    
    for (unsigned int j = 0; j < disparities; ++j)
      local[j] = in[(i+1) * disparities + j];

    out[i] = minPos;
  }
}

#endif
