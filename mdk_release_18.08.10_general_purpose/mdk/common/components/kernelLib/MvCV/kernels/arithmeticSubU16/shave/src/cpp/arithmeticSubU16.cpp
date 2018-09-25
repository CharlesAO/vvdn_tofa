#include "arithmeticSubU16.h"

void mvcvArithmeticSubU16(u16** src1, u16** src2, s16** dst, u32 width)
{
    unsigned int i;
    s16 sub;
    for (i = 0; i < width; i++)
    {
        sub = (src1[0][i] - src2[0][i]);
        dst[0][i] = (s16) (sub);
    }
}
