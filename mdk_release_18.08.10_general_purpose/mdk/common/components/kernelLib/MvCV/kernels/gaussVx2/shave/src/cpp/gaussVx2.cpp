#include "gaussVx2.h"

void mvcvGaussVx2(u8 **inLine,u8 *outLine,int width)
{
    unsigned int gaus_matrix[3] = {16, 64,96 };
    int i;
    for (i = 0; i < width; i++)
    {
        outLine[i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]))>>8;
    }
}
