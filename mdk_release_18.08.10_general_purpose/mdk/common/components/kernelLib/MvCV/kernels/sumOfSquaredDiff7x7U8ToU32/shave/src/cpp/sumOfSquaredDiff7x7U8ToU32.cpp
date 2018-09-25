#include "sumOfSquaredDiff7x7U8ToU32.h"

void mvcvSumOfSquaredDiff7x7U8ToU32(u8** in1, u8** in2, u32** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[7], *lines2[7];
    u32 sum, diff;

	for(i = 0; i < 7; i++)
	{
		lines1[i] = in1[i];
		lines2[i] = in2[i];
	}

    for (i = 0; i < width; i++){
        sum = 0;
        for (x = 0; x < 7; x++)
        {
            for (y = 0; y < 7; y++)
            {
				diff = (u32)lines1[x][y - 3] - (u32)lines2[x][y - 3];
				diff = diff * diff;
                sum += diff;
            }
            lines1[x]++;
            lines2[x]++;
        }

        out[0][i] = (u32)(sum);
    }
    return;
}
