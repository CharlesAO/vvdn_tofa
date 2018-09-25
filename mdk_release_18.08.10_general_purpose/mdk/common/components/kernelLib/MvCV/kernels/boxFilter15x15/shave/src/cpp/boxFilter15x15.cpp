#include "boxFilter15x15.h"

void mvcvBoxfilter15x15(u8** in, u8** out, u32 normalize, u32 width)
{
   unsigned int i;
   int x,y;
    u8* lines[15];
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
    lines[9] = *(in+9);
    lines[10] = *(in+10);
    lines[11] = *(in+11);
    lines[12] = *(in+12);
    lines[13] = *(in+13);
    lines[14] = *(in+14);

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 15; y++)
        {
            for (x = -7; x <= 7; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(u8)(((half)(float)sum)*(half)(0.00444444444444444444444444444444));
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
    return;
}
