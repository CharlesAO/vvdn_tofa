#include <histogram.h>

void mvcvHistogram(u8** in, u32 *hist, u32 width)
{
    u8 *in_line;
    u32 i;
    in_line  = *in;

    for (i=0;i<width; i++)
    {
        hist[in_line[i]]++;

    }
    return;
}
