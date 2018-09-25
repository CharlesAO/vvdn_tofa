#include "sumOfSquaredDiff5x5.h"

u16 mulSaturated(short a, short b);
u16 addSaturated(u16 a, u16 b);

void mvcvSumOfSquaredDiff5x5(u8** in1, u8** in2, u8** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[5], *lines2[5];
    u16 sum, square;
    short diff;

	for(i = 0; i < 5; i++)
	{
		lines1[i] = in1[i];
		lines2[i] = in2[i];
	}

    for (i = 0; i < width; i++){
        sum = 0;
        for (x = 0; x < 5; x++)
        {
            for (y = 0; y < 5; y++)
            {
				      diff = lines1[x][y - 2] - lines2[x][y - 2];
				      square = mulSaturated(diff, diff);
              sum = addSaturated(sum, square);
            }
            lines1[x]++;
            lines2[x]++;
        }
        if (sum >= 255)
            sum = 255;
        out[0][i] = (u8)(sum);
    }
    return;
}

u16 mulSaturated(short a, short b)
{
  u32 s = a * b;
  if(s > 0xffff)
    return 0xffff;
  return s;
}

u16 addSaturated(u16 a, u16 b)
{
  u32 s = a + b;
  if(s > 0xffff)
    return 0xffff;
  return s;
}
