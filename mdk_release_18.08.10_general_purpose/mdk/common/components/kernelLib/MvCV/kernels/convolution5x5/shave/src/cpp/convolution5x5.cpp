#include "convolution5x5.h"

void mvcvConvolution5x5(u8** in, u8** out, half conv[25], u32 inWidth)
{
    int x, y;
    unsigned int i;
    u8* lines[5];
    float sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 5; x++)
        {
            for (y = 0; y < 5; y++)
            {
                sum += (float)(lines[x][y - 2] * conv[x * 5 + y]);
            }
            lines[x]++;
        }
        if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0;
        out[0][i] = (u8)(sum);
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution5x5_opt(u8** in, u8** out, half conv[25], u32 inWidth)
{
    half8 sum;
    half8 clamp255;
    half8 vAligned;
    //input vectors in u8
    uchar8 line0_before, line1_before, line2_before, line3_before, line4_before;
    uchar8 line0, line1, line2, line3, line4;
    uchar8 line0_after, line1_after, line2_after, line3_after, line4_after;
    //input vectors converted to fp16
    half8 line0_f16, line1_f16, line2_f16, line3_f16, line4_f16;
    half8 line0_before_f16, line1_before_f16, line2_before_f16, line3_before_f16, line4_before_f16;
    half8 line0_after_f16, line1_after_f16, line2_after_f16, line3_after_f16, line4_after_f16;    

    u32 loopRuns = (inWidth >> 3) - 1;
    for(int i = 0; i < 8; i++)
        clamp255[i] = 255.0;

    //Initialize lines pointers
    uchar8 *in0 = *(uchar8**)in++;
    uchar8 *in1 = *(uchar8**)in++;
    uchar8 *in2 = *(uchar8**)in++;
    uchar8 *in3 = *(uchar8**)in++;
    uchar8 *in4 = *(uchar8**)in++;
    uchar8 *outputLine = *(uchar8**)out;

    //Load 5 lines
    line0_before = in0[-1];
    line1_before = in1[-1];
    line2_before = in2[-1]; 
    line3_before = in3[-1]; 
    line4_before = in4[-1]; 
    line0 = *in0++;
    line1 = *in1++;
    line2 = *in2++; 
    line3 = *in3++; 
    line4 = *in4++; 
    line0_after = *in0++;
    line1_after = *in1++;
    line2_after = *in2++;
    line3_after = *in3++;
    line4_after = *in4++;

    //convert
    line0_before_f16 = mvuConvert_half8(line0_before);
    line1_before_f16 = mvuConvert_half8(line1_before);
    line2_before_f16 = mvuConvert_half8(line2_before);
    line3_before_f16 = mvuConvert_half8(line3_before);
    line4_before_f16 = mvuConvert_half8(line4_before);
    line0_f16 = mvuConvert_half8(line0);
    line1_f16 = mvuConvert_half8(line1);
    line2_f16 = mvuConvert_half8(line2);
    line3_f16 = mvuConvert_half8(line3);
    line4_f16 = mvuConvert_half8(line4);
    line0_after_f16 = mvuConvert_half8(line0_after);
    line1_after_f16 = mvuConvert_half8(line1_after);
    line2_after_f16 = mvuConvert_half8(line2_after);
    line3_after_f16 = mvuConvert_half8(line3_after);
    line4_after_f16 = mvuConvert_half8(line4_after);
    
     sum   = line0_f16 * conv[2];
     sum  += line1_f16 * conv[7];
     sum  += line2_f16 * conv[12];
     sum  += line3_f16 * conv[17];
     sum  += line4_f16 * conv[22];

    //alignment
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 12);
    sum += vAligned * conv[0];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 12);
    sum += vAligned * conv[5];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 12);
    sum += vAligned * conv[10];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before_f16, line3_f16, 12);
    sum += vAligned * conv[15];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before_f16, line4_f16, 12);
    sum += vAligned * conv[20];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 14);
    sum += vAligned * conv[1];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 14);
    sum += vAligned * conv[6];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 14);
    sum += vAligned * conv[11];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before_f16, line3_f16, 14);
    sum += vAligned * conv[16];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before_f16, line4_f16, 14);
    sum += vAligned * conv[21];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 2);
    sum += vAligned * conv[3];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 2);
    sum += vAligned * conv[8];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 2);
    sum += vAligned * conv[13];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_f16, line3_after_f16, 2);
    sum += vAligned * conv[18];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_f16, line4_after_f16, 2);
    sum += vAligned * conv[23];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 4);
    sum += vAligned * conv[4];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 4);
    sum += vAligned * conv[9];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 4);
    sum += vAligned * conv[14];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_f16, line3_after_f16, 4);
    sum += vAligned * conv[19];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_f16, line4_after_f16, 4);
    sum += vAligned * conv[24];

    //convert output to u8
    sum = __builtin_shave_cmu_clamp0_f16_rr_half8(sum, clamp255);
    uchar8 outputTemp = mvuConvert_uchar8(sum);
   
    //save output
    *outputLine++ = outputTemp;


    for(u32 i = 0; i < loopRuns; i++)
    {
        //copy current -> before
        line0_before_f16 = line0_f16;
        line1_before_f16 = line1_f16;
        line2_before_f16 = line2_f16;
        line3_before_f16 = line3_f16;
        line4_before_f16 = line4_f16;

        //copy after -> current
        line0_f16 = line0_after_f16;
        line1_f16 = line1_after_f16;
        line2_f16 = line2_after_f16;
        line3_f16 = line3_after_f16;
        line4_f16 = line4_after_f16;

        //load
        line0_after = *in0++;
        line1_after = *in1++;
        line2_after = *in2++;        
        line3_after = *in3++;        
        line4_after = *in4++;        

        sum   = line0_f16 * conv[2];
        sum  += line1_f16 * conv[7];
        sum  += line2_f16 * conv[12];
        sum  += line3_f16 * conv[17];
        sum  += line4_f16 * conv[22];
 
        //alignment and summation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 12);
        sum += vAligned * conv[0];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 12);
        sum += vAligned * conv[5];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 12);
        sum += vAligned * conv[10];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before_f16, line3_f16, 12);
        sum += vAligned * conv[15];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before_f16, line4_f16, 12);
        sum += vAligned * conv[20];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before_f16, line0_f16, 14);
        sum += vAligned * conv[1];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before_f16, line1_f16, 14);
        sum += vAligned * conv[6];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before_f16, line2_f16, 14);
        sum += vAligned * conv[11];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before_f16, line3_f16, 14);
        sum += vAligned * conv[16];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before_f16, line4_f16, 14);
        sum += vAligned * conv[21];
        
        //convert
        line0_after_f16 = mvuConvert_half8(line0_after);
        line1_after_f16 = mvuConvert_half8(line1_after);
        line2_after_f16 = mvuConvert_half8(line2_after);
        line3_after_f16 = mvuConvert_half8(line3_after);
        line4_after_f16 = mvuConvert_half8(line4_after);

        //alignment and summation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 2);
        sum += vAligned * conv[3];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 2);
        sum += vAligned * conv[8];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 2);
        sum += vAligned * conv[13];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_f16, line3_after_f16, 2);
        sum += vAligned * conv[18];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_f16, line4_after_f16, 2);
        sum += vAligned * conv[23];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_f16, line0_after_f16, 4);
        sum += vAligned * conv[4];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_f16, line1_after_f16, 4);
        sum += vAligned * conv[9];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_f16, line2_after_f16, 4);
        sum += vAligned * conv[14];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_f16, line3_after_f16, 4);
        sum += vAligned * conv[19];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_f16, line4_after_f16, 4);
        sum += vAligned * conv[24];

        //convert output to u8
        sum = __builtin_shave_cmu_clamp0_f16_rr_half8(sum, clamp255);
        uchar8 outputTemp = mvuConvert_uchar8(sum);

        //save output
        *outputLine++ = outputTemp;
    }   
}
#endif //MOVICOMPILE_OPTIMIZED
