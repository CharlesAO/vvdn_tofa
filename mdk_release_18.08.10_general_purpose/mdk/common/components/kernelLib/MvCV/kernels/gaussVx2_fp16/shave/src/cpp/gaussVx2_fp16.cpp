#include "gaussVx2_fp16.h"

void mvcvGaussVx2_fp16(half **inLine,half **outLine,int width)
{
    half gaus_matrix[3] = {0.0625, 0.25,0.375 };
    int i;
    for (i = 0; i < width; i++)
    {
        outLine[0][i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]));
    }
}
