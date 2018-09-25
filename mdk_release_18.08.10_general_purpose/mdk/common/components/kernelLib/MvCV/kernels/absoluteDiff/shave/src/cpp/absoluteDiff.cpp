#include "absoluteDiff.h"
#include <stdlib.h>

void mvcvAbsoluteDiff(u8** in1, u8** in2, u8** out, u32 width)

{
    u8* in_1;
    u8* in_2;
    in_1= *in1;
    in_2= *in2;
    //check if the two input images have the same size

    u32 j;
    if (sizeof(in1) != sizeof(in2))
    {
    	exit(1);
    }
    else
    {

    	for (j = 0; j < width; j++)
    	{
    		if (in_1[j] > in_2[j])
    		{
    			out[0][j] = in_1[j] - in_2[j];
    		}
    		else
    		{
    			out[0][j] = in_2[j] - in_1[j];
    		}
    	}

    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvAbsoluteDiff_opt(u8** in1, u8** in2, u8** out, u32 width)
{
#ifdef SHAVE_VECTORIZED_C
    // manual vectorization
    // uses v16u8 and intrinsic function for absolute-difference
    uchar16* in_1  = (uchar16*)*in1;
    uchar16* in_2  = (uchar16*)*in2;
    uchar16* out_p = (uchar16*)*out;

    u32 j;
# if !defined SWP_KERNEL || SWP_KERNEL == 1
    // normal loop version
    // cycles/pixel = 0.900229
    for (j = 0; j < width; j += 16)
    {
        *out_p++ = mvuAdiff (*in_1++,*in_2++);
    }
# else
#   if SWP_KERNEL > 3 || SWP_KERNEL < 2
#     error "Invalid SWP_KERNEL value"
#   endif
#   if SWP_KERNEL == 2
    // 2x 'software pipelined' version
    // cycles/pixel = 0.899083
    if( width > 0 ) {
        uchar16 a,b;
                                       a = *in_1++; b = *in_2++;
        for (j = 16; j < width; j += 16)
        {
            *out_p++ = mvuAdiff (a,b); a = *in_1++; b = *in_2++;
        }
            *out_p++ = mvuAdiff (a,b);
    }
#   endif
#   if SWP_KERNEL == 3
    // 3x 'software pipelined' version
    // cycles/pixel = 0.833142
    if( width >= 16 ) {
        uchar16 a,b,c;
                                                a = *in_1++; b = *in_2++;
                            c = mvuAdiff (a,b); a = *in_1++; b = *in_2++;
        for (j = 32; j < width; j += 16)
        {
            *out_p++ = c;   c = mvuAdiff (a,b); a = *in_1++; b = *in_2++;
        }
            *out_p++ = c;   c = mvuAdiff (a,b);
            *out_p++ = c;
    } else if (width != 0) {
        *out_p++ = mvuAdiff (*in_1++, *in_2++);
    }
#   endif
# endif
#else
    // auto vectorized version
    // clang translates (a - b) into 32 bit operations
    // only v4u32 vectorization is obtained this way
    // cycles/pixel = 3.854358
    u8* in_1  = *in1;
    u8* in_2  = *in2;
    u8* out_p = *out;
    u32 j;

#pragma clang loop vectorize(enable)
    for (j = 0; j < width; j++)
    {
        if (in_1[j] > in_2[j])
        {
            out_p[j] = in_1[j] - in_2[j];
        }
        else
        {
            out_p[j] = in_2[j] - in_1[j];
        }
    }
#endif
    return;
}
#endif //MOVICOMPILE_OPTIMIZED
