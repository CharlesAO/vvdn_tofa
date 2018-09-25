#include "erode5x5.h"

void mvcvErode5x5(u8** src, u8** dst, u8** kernel, u32 width)
{
	int j,i1,j1;
	u8 min = 0xFF;
	u8* row[5];


	for(j = 0;j < 5; j++)
		row[j] = src[j];

	// Determine the anchor positions which must be replaces by a maximum value
	for ( j = 0; j < (int)width; j++)
	{
		min = 0xFF;
		// Determine the minimum number into a k*k square
		for(i1 = 0; i1 < 5; i1++ )
		{
			for (j1 = 0; j1 < 5; j1++)
			{
				if(kernel[i1][j1]==1)
				{
					if(min > row[i1][j1 + j - 2]){

						min = row[i1][j1 + j - 2];
					}
				}
			}
		}

		dst[0][j] = min;
	}
}
