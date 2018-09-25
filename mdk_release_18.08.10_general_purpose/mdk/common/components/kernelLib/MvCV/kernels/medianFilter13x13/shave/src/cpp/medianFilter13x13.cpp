#include <stdlib.h>
#include "medianFilter13x13.h"

void mvcvMedianFilter13x13(u32 widthLine, u8 **outLine, u8 ** inLine)
{
    s32 i = 0;
    u8 *out;
    s32 j = 0;
    u32 histogram[256];
    u32 e = 0;
    s32 MED_HEIGHT = 13;
    s32 MED_WIDTH  = 13;
    s32 MED_LIMIT = (MED_WIDTH*MED_HEIGHT)/2 + 1;

    out = *outLine;

	for (i=0; i<256; i++) histogram[i] = 0;
	// build initial histogram
	for (i=0; i<MED_HEIGHT; i++)
	{
		for (j=0; j<MED_WIDTH; j++)
		{
			e =inLine[i][j-(MED_WIDTH>>1)];
			histogram[e]++;
		}
	}
	for (i=0; i<(int)widthLine; i++)
	{
		e = 0;
		j = 0;
		// use histogram
		while (j<256)
		{
			e+=histogram[j];
			if (e<(u32)MED_LIMIT)
			{
				j++;
			} else
			{
				out[i] = j;
				j = 256;
			}
		}
		// substract previous values from histogram
		for (j=0; j<MED_HEIGHT; j++)
		{
			e = inLine[j][i-(MED_WIDTH>>1)];
			histogram[e]--;
		}
		// add next values to histogram
		for (j=0; j<MED_HEIGHT; j++)
		{
			e = inLine[j][i+1+(MED_WIDTH>>1)];
			histogram[e]++;
		}
	}
	return;
}
