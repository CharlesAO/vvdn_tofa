#include "pyrdown.h"

void mvcvPyrdown(u8 **inLine,u8 **out,int width)
{
	unsigned int gaus_matrix[3] = {16, 64,96 };
	int i, j;
	u8 outLine11[1924];
	u8 *outLine1;
	u8 *outLine;

	outLine1 = outLine11;
	outLine1 = outLine1 + 2;

	outLine=*out;

	for (i = -2; i < width +2; i++)
	{
        outLine1[i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]))>>8;
	}


    for (j = 0; j<width;j+=2)
    {
        outLine[j>>1] = (((outLine1[j-2] + outLine1[j+2]) * gaus_matrix[0]) + ((outLine1[j-1] + outLine1[j+1]) * gaus_matrix[1]) + (outLine1[j]  * gaus_matrix[2]))>>8;
    }

	return;
}
