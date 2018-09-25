#include "thresholdKernel.h"

void mvcvThresholdKernel(u8** in, u8** out, u32 width, u32 height, u8 thresh, u32 thresh_type)
{
    u32 i, j;
    u8* in_1;

    in_1 = *in;


    u8 max_value = 0xff;

    for(i = 0; i < height ; i++)
    {
        for(j = 0; j < width; j++)
        {
            switch(thresh_type)
            {
                case Thresh_To_Zero:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    else
                    {
                        out[i][j] = 0;
                    }
                    break;
                case Thresh_To_Zero_Inv:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = 0;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
                case Thresh_To_Binary:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = max_value;
                    }
                    else
                    {
                        out[i][j] = 0;
                    }
                    break;
                case Thresh_To_Binary_Inv:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = 0;
                    }
                    else
                    {
                        out[i][j] = max_value;
                    }
                    break;
                case Thresh_Trunc:
                    if((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = thresh;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
                default:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = thresh;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
            }
        }
    }

    return;
}
