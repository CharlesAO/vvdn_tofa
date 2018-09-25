#include "avgPool7x7xk.h"

void mvcvAvgPool7x7xk(half** srcAddr, half** destAddr, u32 width)
{
    u32 i, j;
    float sum;
    const float f49 = (float)0.02040816326530612;

    for( j = 0; j < width; j++)
    {
        sum = 0;
        for( i = 0; i < 49; i++)
            sum += srcAddr[i][j];
        destAddr[0][j] = (half) (sum * f49);
    }
}
