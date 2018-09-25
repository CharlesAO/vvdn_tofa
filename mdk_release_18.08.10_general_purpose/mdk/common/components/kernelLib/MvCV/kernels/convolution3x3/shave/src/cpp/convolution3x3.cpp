#include "convolution3x3.h"

void mvcvConvolution3x3(u8** in, u8** out, half conv[9], u32 inWidth)
{
    int x,y;
	unsigned int i;
    u8* lines[3];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = 0; y < 3; y++){
                sum += (half)(lines[x][y - 1] * conv[x * 3 + y]);
            }
            lines[x]++;
        }

        if (sum >= (half)255)
            sum = 255.0f;
        if (sum <= (half)0)
            sum = 0.0f;
        out[0][i] = (u8)(sum);
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution3x3_opt(u8** in, u8** out, half conv[9], u32 inWidth)
{
    half8 sum;
    half8 clamp255;
    //input vectors in u8
    uchar8 line0_before, line1_before, line2_before;
    uchar8 line0, line1, line2;
    uchar8 line0_after, line1_after, line2_after;
    //input vectors converted to fp16
    half8 line0_f16, line1_f16, line2_f16;
    half8 line0_before_f16, line1_before_f16, line2_before_f16;
    half8 line0_after_f16, line1_after_f16, line2_after_f16;    

    u32 loopRuns = (inWidth >> 3) - 1;
    for(int i = 0; i < 8; i++)
        clamp255[i] = 255.0;

    //Initialize lines pointers
    uchar8 *in0 = *(uchar8**)in++;
    uchar8 *in1 = *(uchar8**)in++;
    uchar8 *in2 = *(uchar8**)in++;
    uchar8 *outputLine = *(uchar8**)out;

    //Load first 3 lines
    line0_before = in0[-1];
    line1_before = in1[-1];
    line2_before = in2[-1]; 
    line0 = *in0++;
    line1 = *in1++;
    line2 = *in2++; 
    line0_after = *in0++;
    line1_after = *in1++;
    line2_after = *in2++;

    //convert
    line0_before_f16 = mvuConvert_half8(line0_before);
    line1_before_f16 = mvuConvert_half8(line1_before);
    line2_before_f16 = mvuConvert_half8(line2_before);
    line0_f16 = mvuConvert_half8(line0);
    line1_f16 = mvuConvert_half8(line1);
    line2_f16 = mvuConvert_half8(line2);
    line0_after_f16 = mvuConvert_half8(line0_after);
    line1_after_f16 = mvuConvert_half8(line1_after);
    line2_after_f16 = mvuConvert_half8(line2_after);

    
    //alignment
    half8 v0 = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 14);
    half8 v1 = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 14);
    half8 v2 = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 14);
    half8 v3 = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 2);
    half8 v4 = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 2);
    half8 v5 = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 2);
    
    sum  = line0_f16 * conv[1];
    sum += line1_f16 * conv[4];
    sum += line2_f16 * conv[7];
    sum += v0 * conv[0];
    sum += v1 * conv[3];
    sum += v2 * conv[6];
    sum += v3 * conv[2];
    sum += v4 * conv[5];
    sum += v5 * conv[8];

    //convert output to u8
    sum = __builtin_shave_cmu_clamp0_f16_rr_half8(sum, clamp255);
    uchar8 outputTemp = mvuConvert_uchar8(sum);
   
    //save output
    *outputLine++ = outputTemp;


    for(int i = 0; i < (int)loopRuns; i++)
    {
        //copy current -> before
        line0_before_f16 = line0_f16;
        line1_before_f16 = line1_f16;
        line2_before_f16 = line2_f16;

        //copy after -> current
        line0_f16 = line0_after_f16;
        line1_f16 = line1_after_f16;
        line2_f16 = line2_after_f16;

        //load
        line0_after = *in0++;
        line1_after = *in1++;
        line2_after = *in2++;        
 
        //alignment
        half8 v0 = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 14);
        half8 v1 = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 14);
        half8 v2 = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 14);
        
        //convert
        line0_after_f16 = mvuConvert_half8(line0_after);
        line1_after_f16 = mvuConvert_half8(line1_after);
        line2_after_f16 = mvuConvert_half8(line2_after);

        //alignment
        half8 v3 = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 2);
        half8 v4 = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 2);
        half8 v5 = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 2);
        
        //add the elements from the 3x3 square (those are already multiplied with the corresponding coef)
        sum  = line0_f16 * conv[1];
        sum += line1_f16 * conv[4];
        sum += line2_f16 * conv[7];
        sum += v0 * conv[0];
        sum += v1 * conv[3];
        sum += v2 * conv[6];
        sum += v3 * conv[2];
        sum += v4 * conv[5];
        sum += v5 * conv[8];

        //convert output to u8
        sum = __builtin_shave_cmu_clamp0_f16_rr_half8(sum, clamp255);
        uchar8 outputTemp = mvuConvert_uchar8(sum);

        //save output
        *outputLine++ = outputTemp;
    }   
}
#endif //MOVICOMPILE_OPTIMIZED