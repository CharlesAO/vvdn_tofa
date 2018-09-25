#include "convSeparable5x5Fp16ToFp16.h"

void mvcvConvSeparable5x5Fp16ToFp16(half** out, half** in, half conv[3], u32 inWidth)
{
    int i;
    half* lines[5];
    float sum = 0.0f;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		sum = ((float)(lines[0][i-2] + lines[4][i-2]) * (float)conv[0] + (float)(lines[1][i-2] + lines[3][i-2]) * (float)conv[1] + (float)lines[2][i-2] * (float)conv[2] +
			   (float)(lines[0][i+2] + lines[4][i+2]) * (float)conv[0] + (float)(lines[1][i+2] + lines[3][i+2]) * (float)conv[1] + (float)lines[2][i+2] * (float)conv[2]) * (float)conv[0] +
			  ((float)(lines[0][i-1] + lines[4][i-1]) * (float)conv[0] + (float)(lines[1][i-1] + lines[3][i-1]) * (float)conv[1] + (float)lines[2][i-1] * (float)conv[2] +
			   (float)(lines[0][i+1] + lines[4][i+1]) * (float)conv[0] + (float)(lines[1][i+1] + lines[3][i+1]) * (float)conv[1] + (float)lines[2][i+1] * (float)conv[2]) * (float)conv[1] +
			  ((float)(lines[0][i  ] + lines[4][i  ]) * (float)conv[0] + (float)(lines[1][i  ] + lines[3][i  ]) * (float)conv[1] + (float)lines[2][i  ] * (float)conv[2]) * (float)conv[2];

        out[0][i] = (half)(sum);
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvSeparable5x5Fp16ToFp16_opt(half** out, half** in, half conv[3], u32 inWidth)
{
    half8 sum, sum1, sum2, sum3, sum4, sum5, tempSum;
    half8 v1Aligned, v2Aligned, vAligned;
    half8 coef0 = conv[0];
    half8 coef1 = conv[1];
    half8 coef2 = conv[2];

    //input vectors
    half8 line0_before, line1_before, line2_before, line3_before, line4_before;
    half8 line0, line1, line2, line3, line4;
    half8 line0_after, line1_after, line2_after, line3_after, line4_after;

    u32 loopRuns = (inWidth >> 3) - 1;

    //Initialize lines pointers
    half8 *in0 = *(half8**)in++;
    half8 *in1 = *(half8**)in++;
    half8 *in2 = *(half8**)in++;
    half8 *in3 = *(half8**)in++;
    half8 *in4 = *(half8**)in++;
    half8 *outputLine = *(half8**)out;

    //load
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


    // sum1 = ((lines[0][i] + lines[4][i]) * conv[0] +
    //         (lines[1][i] + lines[3][i]) * conv[1] +
    //          lines[2][i] * conv[2])
    sum1 = line2 * coef2;
    tempSum = line0 + line4;
    sum1 += tempSum * coef0;
    tempSum = line1 + line3;
    sum1  += tempSum * coef1;
    sum = sum1 * coef2;

    // sum2 = (lines[0][i-1] + lines[4][i-1]) * conv[0] +
    //        (lines[1][i-1] + lines[3][i-1]) * conv[1] +
    //         lines[2][i-1] * conv[2]
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
    sum2 = vAligned * coef2;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 12);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 12);
    tempSum = v1Aligned + v2Aligned;
    sum2 += tempSum * coef0;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 12);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 12);
    tempSum = v1Aligned + v2Aligned;
    sum2 += tempSum * coef1;
    sum += sum2 * coef0;

    // sum3 = (lines[0][i-2] + lines[4][i-2]) * conv[0] +
    //        (lines[1][i-2] + lines[3][i-2]) * conv[1] +
    //         lines[2][i-2] * conv[2]
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
    sum3 = vAligned * coef2;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 14);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 14);
    tempSum = v1Aligned + v2Aligned;
    sum3 += tempSum * coef0;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
    tempSum = v1Aligned + v2Aligned;
    sum3 += tempSum * coef1;
    sum +=  sum3 * coef1;

    // sum4 = ((lines[0][i+1] + lines[4][i+1]) * conv[0] +
    //         (lines[1][i+1] + lines[3][i+1]) * conv[1] +
    //          lines[2][i+1] * conv[2])
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
    sum4 = vAligned * coef2;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 2);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 2);
    tempSum = v1Aligned + v2Aligned;
    sum4 += tempSum * coef0;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
    tempSum = v1Aligned + v2Aligned;
    sum4 += tempSum * coef1;
    sum += sum4 * coef1;

    // sum5 = ((lines[0][i+2] + lines[4][i+2]) * conv[0] +
    //         (lines[1][i+2] + lines[3][i+2]) * conv[1] +
    //          lines[2][i+2] * conv[2])
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
    sum5 = vAligned * coef2;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 4);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 4);
    tempSum = v1Aligned + v2Aligned;
    sum5 += tempSum * coef0;
    v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 4);
    v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 4);
    tempSum = v1Aligned + v2Aligned;
    sum5 += tempSum * coef1;
    sum += sum5 * coef0;

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

        // sum1 = ((lines[0][i] + lines[4][i]) * conv[0] +
        //         (lines[1][i] + lines[3][i]) * conv[1] +
        //          lines[2][i] * conv[2])
        sum1 = line2 * coef2;
        tempSum = line0 + line4;
        sum1 += tempSum * coef0;
        tempSum = line1 + line3;
        sum1  += tempSum * coef1;
        sum = sum1 * coef2;

        // sum2 = (lines[0][i-1] + lines[4][i-1]) * conv[0] +
        //        (lines[1][i-1] + lines[3][i-1]) * conv[1] +
        //         lines[2][i-1] * conv[2]
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
        sum2 = vAligned * coef2;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 12);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 12);
        tempSum = v1Aligned + v2Aligned;
        sum2 += tempSum * coef0;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 12);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 12);
        tempSum = v1Aligned + v2Aligned;
        sum2 += tempSum * coef1;
        sum += sum2 * coef0;

        // sum3 = (lines[0][i-2] + lines[4][i-2]) * conv[0] +
        //        (lines[1][i-2] + lines[3][i-2]) * conv[1] +
        //         lines[2][i-2] * conv[2]
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
        sum3 = vAligned * coef2;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0_before, line0, 14);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4_before, line4, 14);
        tempSum = v1Aligned + v2Aligned;
        sum3 += tempSum * coef0;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
        tempSum = v1Aligned + v2Aligned;
        sum3 += tempSum * coef1;
        sum +=  sum3 * coef1;

        // sum4 = ((lines[0][i+1] + lines[4][i+1]) * conv[0] +
        //         (lines[1][i+1] + lines[3][i+1]) * conv[1] +
        //          lines[2][i+1] * conv[2])
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
        sum4 = vAligned * coef2;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 2);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 2);
        tempSum = v1Aligned + v2Aligned;
        sum4 += tempSum * coef0;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
        tempSum = v1Aligned + v2Aligned;
        sum4 += tempSum * coef1;
        sum += sum4 * coef1;

        // sum5 = ((lines[0][i+2] + lines[4][i+2]) * conv[0] +
        //         (lines[1][i+2] + lines[3][i+2]) * conv[1] +
        //          lines[2][i+2] * conv[2])
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
        sum5 = vAligned * coef2;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line0, line0_after, 4);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line4, line4_after, 4);
        tempSum = v1Aligned + v2Aligned;
        sum5 += tempSum * coef0;
        v1Aligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 4);
        v2Aligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 4);
        tempSum = v1Aligned + v2Aligned;
        sum5 += tempSum * coef1;
        sum += sum5 * coef0;

        //save output
        *outputLine++ = sum;
    }
}
#endif //MOVICOMPILE_OPTIMIZED