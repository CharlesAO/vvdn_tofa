#include "gauss.h"

void mvcvGauss(u8** inLine, u8** out, u32 width)
{
	fp32 gaus_matrix[3] = {1.4, 3.0, 3.8 };
	int i;
	fp32 out1, out2, out3, out4, out5;
	u8 *outLine;


	outLine=*out;

	for (i = 0; i < (int)width; i++)
	{
        out1 = (((inLine[0][i-2] + inLine[4][i-2]) * gaus_matrix[0]) + ((inLine[1][i-2] + inLine[3][i-2]) * gaus_matrix[1]) + (inLine[2][i-2]  * gaus_matrix[2]));
        out2 = (((inLine[0][i-1] + inLine[4][i-1]) * gaus_matrix[0]) + ((inLine[1][i-1] + inLine[3][i-1]) * gaus_matrix[1]) + (inLine[2][i-1]  * gaus_matrix[2]));
        out3 = (((inLine[0][i-0] + inLine[4][i-0]) * gaus_matrix[0]) + ((inLine[1][i-0] + inLine[3][i-0]) * gaus_matrix[1]) + (inLine[2][i-0]  * gaus_matrix[2]));
        out4 = (((inLine[0][i+1] + inLine[4][i+1]) * gaus_matrix[0]) + ((inLine[1][i+1] + inLine[3][i+1]) * gaus_matrix[1]) + (inLine[2][i+1]  * gaus_matrix[2]));
        out5 = (((inLine[0][i+2] + inLine[4][i+2]) * gaus_matrix[0]) + ((inLine[1][i+2] + inLine[3][i+2]) * gaus_matrix[1]) + (inLine[2][i+2]  * gaus_matrix[2]));
		outLine[i] = (u8)((((out1 + out5) * gaus_matrix[0]) + ((out2 + out4) * gaus_matrix[1]) + (out3  * gaus_matrix[2]))/159);
	}

    return;
}
