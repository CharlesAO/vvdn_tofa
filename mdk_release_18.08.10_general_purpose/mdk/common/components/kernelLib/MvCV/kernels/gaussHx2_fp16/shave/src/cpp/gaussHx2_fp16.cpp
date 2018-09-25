#include "gaussHx2_fp16.h"

void mvcvGaussHx2_fp16(half **inLine,half **outLine,int width)
{
    half gaus_matrix[3] = {0.0625, 0.25,0.375 };
    int i;
    for (i = 0; i<width;i+=2)
    {
        outLine[0][i>>1] = (((inLine[0][i-2] + inLine[0][i+2]) * gaus_matrix[0]) + ((inLine[0][i-1] + inLine[0][i+1]) * gaus_matrix[1]) + (inLine[0][i]  * gaus_matrix[2]));
    }
}
