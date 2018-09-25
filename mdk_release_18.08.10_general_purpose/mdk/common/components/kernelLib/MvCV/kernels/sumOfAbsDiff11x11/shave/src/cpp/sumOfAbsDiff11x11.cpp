#include "sumOfAbsDiff11x11.h"

void mvcvSumOfAbsDiff11x11(u8** in1, u8** in2, u8** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[11], *lines2[11];
    int sum, diff;
	
	for(i = 0; i < 11; i++)
	{
		lines1[i] = in1[i];
		lines2[i] = in2[i];
	}

    for (i = 0; i < width; i++){
        sum = 0;
        for (x = 0; x < 11; x++)
        {
            for (y = 0; y < 11; y++)
            {
               diff = lines1[x][y - 5] - lines2[x][y - 5];
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
