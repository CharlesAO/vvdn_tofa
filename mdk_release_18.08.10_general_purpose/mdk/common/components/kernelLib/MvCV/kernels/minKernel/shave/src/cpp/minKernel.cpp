#include "minKernel.h"

void mvcvMinKernel(u8* in, u8* minVal, u32 width)
{
    u8 minV = 0xFF;
    u32 j;

    for(j = 0; j < width; j++)
    {
        if (in[j] < minV)
        {
            minV = in[j];
        }
    }

    if (minVal != NULL)
    {
        *minVal = minV;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED

void mvcvMinKernel_opt(unsigned char * in, unsigned char * minVal, unsigned int width) {
  unsigned char minV = 0xFF;

  for(unsigned int j = 0; j < width; j++)
    if (in[j] < minV)
      minV = in[j];

  if (minVal)
    *minVal = minV;
}
#endif
