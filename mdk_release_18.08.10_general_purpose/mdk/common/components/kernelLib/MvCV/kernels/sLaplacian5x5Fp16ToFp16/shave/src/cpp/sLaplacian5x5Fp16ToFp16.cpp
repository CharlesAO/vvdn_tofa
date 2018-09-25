#include "sLaplacian5x5Fp16ToFp16.h"

void mvcvSLaplacian5x5Fp16ToFp16(half** in, half** out, u32 inWidth)
{
    int i,x,y;
    half* lines[5];
	half sum;
	int lapl[25]={ 0, 0, -1, 0, 0, 0, -1, -2, -1, 0, -1, -2, 17, -2, -1, 0, -1, -2, -1, 0, 0, 0, -1, 0, 0};
		
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
	
	//Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
	
		sum = 0.0f;
        for (x = 0; x < 5; x++){
            for (y = -2; y < 3; y++){
                sum += lines[x][y] * (half)lapl[x * 5 + y + 2];
            }
            lines[x]++;
        }

        /*if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;*/
        out[0][i] = sum;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvSLaplacian5x5Fp16ToFp16_opt(half** in, half** out, u32 inWidth)
{
    int lapl[25]={  0,  0, -1,  0,  0, 
                    0, -1, -2, -1,  0, 
                   -1, -2, 17, -2, -1,
                    0, -1, -2, -1,  0, 
                    0,  0, -1,  0,  0};
        
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

    //Load first 3 lines
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

     sum   = -(line0);
     sum  += line1 * lapl[7];
     sum  += line2 * lapl[12];
     sum  += line3 * lapl[17];
     sum  -= line4;

    //alignment
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
    sum -= vAligned;
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
    sum -= vAligned;
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
    sum += vAligned * lapl[11];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
    sum -= vAligned;

    //copy current -> before
    line0_before = line0;
    line1_before = line1;
    line2_before = line2;
    line3_before = line3;
    line4_before = line4;

    line0 = line0_after;
    line0_after = *in0++;
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
    line1 = line1_after;
    line1_after = *in1++;
    sum -= vAligned;
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
    line2 = line2_after;
    line2_after = *in2++; 
    sum += vAligned * lapl[13];
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
    line3 = line3_after;
    line3_after = *in3++;
    sum -= vAligned;
    vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
    line4 = line4_after;
    line4_after = *in4++;
    sum -= vAligned;

    //save output
    *outputLine++ = sum;


    for(int i = 0; i < (int)loopRuns; i++)
    {            

        sum   = -(line0);
        sum  += line1 * lapl[7];
        sum  += line2 * lapl[12];
        sum  += line3 * lapl[17];
        sum  -= line4;
 
        //alignment and sumsmation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 12);
        sum -= vAligned;
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1_before, line1, 14);
        sum -= vAligned;
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2_before, line2, 14);
        sum += vAligned * lapl[11];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3_before, line3, 14);
        sum -= vAligned;

        //copy current -> before
        line0_before = line0;
        line1_before = line1;
        line2_before = line2;
        line3_before = line3;
        line4_before = line4;

        line0 = line0_after;
        line0_after = *in0++;

        //alignment and summation
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line1, line1_after, 2);
        line1 = line1_after;
        line1_after = *in1++;
        sum -= vAligned;
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 2);
        line2 = line2_after;
        line2_after = *in2++;
        sum += vAligned * lapl[13];
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line3, line3_after, 2);
        line3 = line3_after;
        line3_after = *in3++;
        sum -= vAligned;
        vAligned = __builtin_shave_cmu_alignvec_rri_short8(line2, line2_after, 4);
        line4 = line4_after;
        line4_after = *in4++;
        sum -= vAligned;
     
        //save output
        *outputLine++ = sum;
    }   
}
#endif //MOVICOMPILE_OPTIMIZED