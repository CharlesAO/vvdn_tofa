#include "boxFilter9x9.h"

void mvcvBoxfilter9x9(u8** in, u8** out, u32 normalize, u32 width)
{
    unsigned int i;
	int x,y;
    u8* lines[9];
    unsigned int sum;
    unsigned short* aux;
    aux=(unsigned short *)(*out);

    //Initialize lines pointers
    lines[0] = *in;
    lines[1] = *(in+1);
    lines[2] = *(in+2);
    lines[3] = *(in+3);
    lines[4] = *(in+4);
    lines[5] = *(in+5);
    lines[6] = *(in+6);
    lines[7] = *(in+7);
    lines[8] = *(in+8);

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 9; y++)
        {
            for (x = -4; x <= 4; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(u8)(((half)(float)sum)*(half)(0.01234567901234567901234567901235));
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
    return;
}
