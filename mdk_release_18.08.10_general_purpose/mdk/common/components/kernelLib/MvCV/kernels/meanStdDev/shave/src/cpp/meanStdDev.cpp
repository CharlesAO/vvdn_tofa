#include "meanStdDev.h"
#include <math.h>

void mvcvMeanstddev(u8** in, float *mean, float *stddev, u32 width)
{
    float sum;
    float square_sum;
    float f0, f1;
    u8    *line = in[0];
    int   i;

    sum = square_sum = 0.0f;

    for(i = 0; i < (int)width; i++)
    {
        sum        += (float)line[i];
        square_sum += (((float)line[i]) * ((float)line[i]));
    }

    *mean = sum / (float)width;
    f0 = square_sum / (float)width;
    f1 = (*mean) * (*mean);
    if(__builtin_isgreater(f0, f1) == 1)
    {
        *stddev = sqrt(f0 - f1);
    }
    else
    {
        *stddev = 0;
    }
	return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvMeanstddev_opt(unsigned char ** __restrict in, float * __restrict mean, float * __restrict stddev, unsigned int width)
{
  uchar8 * __restrict line = (uchar8 *) in[0];

  int sum = 0;
  int squareSum = 0;
  float oneOverWidth = 1.0f / (float) width;

  ushort8 val = mvuConvert_ushort8(line[0]);
  
  #pragma unroll 4
  for(int i = 0; i < (int) (width/8); i++) {
    sum += __builtin_shave_sau_sumx_u16_r(val);
    squareSum += __builtin_shave_sau_sumx_u16_r(val * val);
    val = mvuConvert_ushort8(line[i+1]);
  }

  float localMean = (float) sum * oneOverWidth;
  float f0 = (float) squareSum * oneOverWidth;
  float f1 = localMean * localMean;
  
  *stddev = (float) __builtin_shave_sau_sqt_f16_l_r((short float) (f0 - f1));
  *mean = localMean;
}
#endif
