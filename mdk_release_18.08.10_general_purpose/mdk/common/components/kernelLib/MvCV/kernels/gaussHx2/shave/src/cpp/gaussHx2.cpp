#include "gaussHx2.h"

void mvcvGaussHx2(u8 *inLine,u8 *outLine,int width)
{
    unsigned int gaus_matrix[3] = {16, 64,96 };
    int i;
    for (i = 0; i<(width<<1);i+=2)
    {
        outLine[i>>1] = (((inLine[i-2] + inLine[i+2]) * gaus_matrix[0]) + ((inLine[i-1] + inLine[i+1]) * gaus_matrix[1]) + (inLine[i]  * gaus_matrix[2]))>>8;
    }
}
