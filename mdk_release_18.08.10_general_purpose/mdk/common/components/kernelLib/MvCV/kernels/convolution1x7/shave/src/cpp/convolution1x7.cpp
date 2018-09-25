#include "convolution1x7.h"

void mvcvConvolution1x7(u8 **in, u8 **out, half conv[7], u32 inWidth)
{
    u32 i;
	s8 y;
    float sum = 0.0;
    u8    *input  = *in;
    u8    *output = *out;
	
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 7; y++)
        {
            sum += ((float)input[y - 3]) * conv[y];
        }
        input++;

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = (u8)(sum);
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution1x7_opt( u8 ** inputs, u8 ** outputs, half conv[7], u32 inWidth)
{
    u32 i, y;
    UNUSED(y);
    half8 sum ; 
    uchar8 * output;
    half8 fake_input, fake_conv;
    UNUSED(fake_input);
    u8 *input = *inputs;
    output = ( uchar8 *)(*outputs);
    fake_conv = *(( half8 *)conv );
    fake_conv[ 7 ] = 0.0h ;
    for (i = 0; i < inWidth; i += 8)
    {
        sum[0] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i - 3])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[1] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i - 2])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[2] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i - 1])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[3] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)( (&input[i]) )))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[4] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i + 1])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[5] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i + 2])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[6] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i + 3])))) * fake_conv ), 0.0h , 255.0h );
        // Sum and clamp
        sum[7] = __builtin_shave_cmu_clampab_f16_rrr_half( __builtin_shave_sau_sumx_f16_r ( (mvuConvert_half8 ( *(( uchar8 *)(&input[i + 4])))) * fake_conv ), 0.0h , 255.0h );
        output[i >> 3] = mvuConvert_uchar8(sum);
    }
    return ;
}
#endif //MOVICOMPILE_OPTIMIZED
