#include "boxFilter11x11.h"

void mvcvBoxfilter11x11(u8** in, u8** out, u32 normalize, u32 width)
{
    unsigned int i;
	int x,y;
    u8* lines[11];
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

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 11; y++)
        {
            for (x = -5; x <= 5; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(u8)(((half)(float)sum)*(half)(0.00826446280991735537190082644628));
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
    return;
}
