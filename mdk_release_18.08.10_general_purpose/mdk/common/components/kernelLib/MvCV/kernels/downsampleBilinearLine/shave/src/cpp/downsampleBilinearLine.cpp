#include "downsampleBilinearLine.h"

void mvcvDownsampleBilinearLine(u16 *inLine, u32 *tempLine, u16 *outLine, u32 inWidth, u32 select)
{
    u32 i, j;
    
    if(select==0)
    {
        for(i=0, j=0;i<inWidth;i+=2, j++)
        {
            tempLine[j]=inLine[i] + inLine[i+1];
        }
    }
    else
    {
        for(i=0, j=0;i<inWidth;i+=2, j++)
        {
            outLine[j]=(u16)((inLine[i] + inLine[i+1] + tempLine[j]+2)>>2);
        }
    }
}
