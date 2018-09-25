#include "convolution5x5Fp16ToFp16.h"

void mvcvConvolution5x5Fp16ToFp16(half** in, half** out, half conv[25], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[5];
    half sum;


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
                sum += lines[x][y - 2] * conv[x * 5 + y];
            }
            lines[x]++;
        }
       /* if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0; */
        out[0][i] = sum;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution5x5Fp16ToFp16_opt(half** in, half** out, half conv[25], u32 inWidth)
{
    half8 sum;
    half8 clamp255;
    half8 vAligned;
    //input vectors 
    half8 line0_before, line1_before, line2_before, line3_before, line4_before;
    half8 line0, line1, line2, line3, line4;
    half8 line0_after, line1_after, line2_after, line3_after, line4_after;
 

    u32 loopRuns = (inWidth >> 3) - 1;
    for(int i = 0; i < 8; i++)
        clamp255[i] = 255.0;

    //Initialize lines pointers
    half8 *in0 = *(half8**)in++;
    half8 *in1 = *(half8**)in++;
    half8 *in2 = *(half8**)in++;
    half8 *in3 = *(half8**)in++;
    half8 *in4 = *(half8**)in++;
    half8 *outputLine = *(half8**)out;

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

     sum   = line0 * conv[2];
     sum  += line1 * conv[7];
     sum  += line2 * conv[12];
     sum  += line3 * conv[17];
     sum  += line4 * conv[22];

    //alignment
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 12);
    sum += vAligned * conv[0];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 12);
    sum += vAligned * conv[5];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
    sum += vAligned * conv[10];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 12);
    sum += vAligned * conv[15];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 12);
    sum += vAligned * conv[20];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 14);
    sum += vAligned * conv[1];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
    sum += vAligned * conv[6];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
    sum += vAligned * conv[11];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
    sum += vAligned * conv[16];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 14);
    sum += vAligned * conv[21];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 2);
    sum += vAligned * conv[3];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
    sum += vAligned * conv[8];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
    sum += vAligned * conv[13];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
    sum += vAligned * conv[18];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 2);
    sum += vAligned * conv[23];

    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 4);
    sum += vAligned * conv[4];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 4);
    sum += vAligned * conv[9];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
    sum += vAligned * conv[14];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 4);
    sum += vAligned * conv[19];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 4);
    sum += vAligned * conv[24];
   
    //save output
    *outputLine++ = sum;


    for(int i = 0; i < (int)loopRuns; i++)
    {
        //copy current -> before
        line0_before = line0;
        line1_before = line1;
        line2_before = line2;
        line3_before = line3;
        line4_before = line4;

        //copy after -> current
        line0 = line0_after;
        line1 = line1_after;
        line2 = line2_after;
        line3 = line3_after;
        line4 = line4_after;

        //load
        line0_after = *in0++;
        line1_after = *in1++;
        line2_after = *in2++;        
        line3_after = *in3++;        
        line4_after = *in4++;        

        sum   = line0 * conv[2];
        sum  += line1 * conv[7];
        sum  += line2 * conv[12];
        sum  += line3 * conv[17];
        sum  += line4 * conv[22];
 
        //alignment and summation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 12);
        sum += vAligned * conv[0];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 12);
        sum += vAligned * conv[5];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
        sum += vAligned * conv[10];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 12);
        sum += vAligned * conv[15];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 12);
        sum += vAligned * conv[20];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 14);
        sum += vAligned * conv[1];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
        sum += vAligned * conv[6];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
        sum += vAligned * conv[11];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
        sum += vAligned * conv[16];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 14);
        sum += vAligned * conv[21];

        //alignment and summation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 2);
        sum += vAligned * conv[3];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
        sum += vAligned * conv[8];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
        sum += vAligned * conv[13];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
        sum += vAligned * conv[18];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 2);
        sum += vAligned * conv[23];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 4);
        sum += vAligned * conv[4];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 4);
        sum += vAligned * conv[9];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
        sum += vAligned * conv[14];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 4);
        sum += vAligned * conv[19];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 4);
        sum += vAligned * conv[24];

        //save output
        *outputLine++ = sum;
    }   
}
#endif //MOVICOMPILE_OPTIMIZED