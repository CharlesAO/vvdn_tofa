#include "extractDescriptor64_freakA.h"
#include "genPairsFREAK.h"

void mvcvExtractDescriptor64_freakA(unsigned short *in_pixel_array,  unsigned char *out_descriptor)
{
    // now iterate through all the pairings
    int  shifter, i;
    unsigned short t1, t2;
    int  noPairs = 512;

    for(i = 0; i < 64; i++)
    {
        out_descriptor[i] = 0;
    }

    shifter = 0;
    for(i = 0; i < noPairs; ++i)
    {
        t1 = in_pixel_array[shortPairsFREAK[i].i];
        t2 = in_pixel_array[shortPairsFREAK[i].j];
        if (t1 >= t2)
        {
            *out_descriptor |= ((1) << shifter);
        } // else already initialized with zero
        // take care of the iterators:
        ++shifter;
        if (shifter == 8)
        {
            shifter = 0;
            ++out_descriptor;
        }
    }
}
