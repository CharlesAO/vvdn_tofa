#include "erode3x3.h"

void mvcvErode3x3(u8** src, u8** dst, u8** kernel, u32 width)
{
	int j,i1,j1;
	u8 min = 0xFF;
	u8* row[3];

	for(j = 0;j < 3; j++)
			row[j] = src[j];

	// Determine the anchor positions which must be replaces by a maximum value
	for ( j = 0; j < (int)width; j++)
	{
		min = 0xFF;
		// Determine the minimum number into a k*k square
		for(i1 = 0; i1 < 3; i1++ )
		{
			for (j1 = 0; j1 < 3; j1++)
			{
				if(kernel[i1][j1]==1)
				{
					if(min > row[i1][j1 + j - 1]){

						min = row[i1][j1 + j - 1];
					}
				}
			}
		}

		dst[0][j] = min;
	}
}
