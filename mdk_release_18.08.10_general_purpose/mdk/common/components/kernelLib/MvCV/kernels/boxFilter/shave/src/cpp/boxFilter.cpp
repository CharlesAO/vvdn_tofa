#include "boxFilter.h"

void mvcvBoxfilter(u8** in, u8** out, u32 k_size, u32 normalize, u32 width)
{
    unsigned int i,x,y;
    u8* lines[15];
    float sum;
    unsigned short* aux;
    aux=(unsigned short *)(*out);

    //initialize line pointers
    for(i=0;i<k_size;i++)
    {
        lines[i]=*(in+i);
    }

    //compute the actual out pixel
    for (i=0;i<width;i++)
    {
        sum=0.0f;
        for (x=0;x<k_size;x++)
        {
            for (y=0;y<k_size;y++)
            {
                sum+=(float)lines[x][y];
            }
            lines[x]++;
        }
        if(normalize == 1) *(*out+i)=(u8)(sum/((float)k_size*k_size));
        else *(aux+i)=(unsigned short)sum;
    }
    return;
}
