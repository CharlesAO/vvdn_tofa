#include "sumOfAbsDiff5x5.h"

void mvcvSumOfAbsDiff5x5(u8** in1, u8** in2, u8** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[5], *lines2[5];
    int sum, diff;

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
				if(diff < 0)
					diff = 0 - diff;
                sum += diff;
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
