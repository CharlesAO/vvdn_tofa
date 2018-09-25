#include "dilate5x5.h"

void mvcvDilate5x5(u8** src, u8** dst, u8** kernel, u32 width)
{
	int j,i1,j1;
	u8 max = 0;
	u8* row[5];


	for(j = 0;j < 5; j++)
			row[j] = src[j];

	// Determine the anchor positions which must be replaces by a maximum value
	for ( j = 0; j < (int)width; j++)
	{
		max = 0;
		// Determine the maximum number into a k*k square
		for(i1 = 0; i1 < 5; i1++ )
		{
			for (j1 = 0; j1 < 5; j1++)
			{
				if(kernel[i1][j1]==1)
				{
					if(max < row[i1][j1 + j - 2]){

						max = row[i1][j1 + j - 2];
					}
				}
			}
		}

		dst[0][j] = max;
	}
}
