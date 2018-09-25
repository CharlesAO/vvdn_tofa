#include "dilate7x7.h"

void mvcvDilate7x7(u8** src, u8** dst, u8** kernel, u32 width)
{
	int j, i1, j1;
	u8 max = 0;
	u8* row[7];


	for(j = 0;j < 7; j++)
			row[j] = src[j];

	// Determine the anchor positions which must be replaces by a maximum value
	for ( j = 0; j < (int)width; j++)
	{
		max = 0;
		// Determine the maximum number into a k*k square
		for(i1 = 0; i1 < 7; i1++ )
		{
			for (j1 = 0; j1 < 7; j1++)
			{
				if(kernel[i1][j1]==1)
				{
					if(max < row[i1][j1 + j - 3]){

						max = row[i1][j1 + j - 3];
					}
				}
			}
		}

		dst[0][j] = max;
	}
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvDilate7x7_opt(u8** src, u8** dst, u8** kernel, u32 width)
{
    //input vectors
    uchar16 line0_before, line1_before, line2_before, line3_before, line4_before, line5_before, line6_before;
    uchar16 line0, line1, line2, line3, line4, line5, line6;
    uchar16 line0_after, line1_after, line2_after, line3_after, line4_after, line5_after, line6_after;
    uchar16 kernel0, kernel1, kernel2, kernel3, kernel4, kernel5, kernel6;

    //Initialize lines pointers
    uchar16 *in0 = *(uchar16**)src++;
    uchar16 *in1 = *(uchar16**)src++;
    uchar16 *in2 = *(uchar16**)src++;
    uchar16 *in3 = *(uchar16**)src++;
    uchar16 *in4 = *(uchar16**)src++;
    uchar16 *in5 = *(uchar16**)src++;
    uchar16 *in6 = *(uchar16**)src;
    uchar16 *inK0 = *(uchar16**)kernel++;
    uchar16 *inK1 = *(uchar16**)kernel++;
    uchar16 *inK2 = *(uchar16**)kernel++;
    uchar16 *inK3 = *(uchar16**)kernel++;
    uchar16 *inK4 = *(uchar16**)kernel++;
    uchar16 *inK5 = *(uchar16**)kernel++;
    uchar16 *inK6 = *(uchar16**)kernel;
    uchar16 *outputLine = (uchar16*)dst[0];

    //load padding elements for each line
    line0_before = in0[-1];
    line1_before = in1[-1];
    line2_before = in2[-1]; 
    line3_before = in3[-1]; 
    line4_before = in4[-1]; 
    line5_before = in5[-1]; 
    line6_before = in6[-1]; 
    kernel0 = inK0[0];
    kernel1 = inK1[0];
    kernel2 = inK2[0];
    kernel3 = inK3[0];
    kernel4 = inK4[0];
    kernel5 = inK5[0];
    kernel6 = inK6[0];

    //load first 16 elements from each line
    line0 = *in0++;
    line1 = *in1++;
    line2 = *in2++; 
    line3 = *in3++; 
    line4 = *in4++; 
    line5 = *in5++; 
    line6 = *in6++; 
    
    //load the next 16 elements from each line
    line0_after = *in0++;
    line1_after = *in1++;
    line2_after = *in2++;
    line3_after = *in3++;
    line4_after = *in4++;
    line5_after = *in5++;
    line6_after = *in6++; 



	for(u32 i = 0; i < (width/16); i++)
	{
		uchar16 v0, v1, v2, v3, v4, v5, v6, max;

		//line0
		v0 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 15);

        v0 = v0 * kernel0[0];
        v1 = v1 * kernel0[1];
        v2 = v2 * kernel0[2];
        v3 = line0 * kernel0[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(v0, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 3);

    	v4 = v4 * kernel0[4];
        v5 = v5 * kernel0[5];
        v6 = v6 * kernel0[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line1
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 15);

        v0 = v0 * kernel1[0];
        v1 = v1 * kernel1[1];
        v2 = v2 * kernel1[2];
        v3 = line1 * kernel1[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 3);

    	v4 = v4 * kernel1[4];
        v5 = v5 * kernel1[5];
        v6 = v6 * kernel1[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line2
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 15);

        v0 = v0 * kernel2[0];
        v1 = v1 * kernel2[1];
        v2 = v2 * kernel2[2];
        v3 = line2 * kernel2[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 3);

    	v4 = v4 * kernel2[4];
        v5 = v5 * kernel2[5];
        v6 = v6 * kernel2[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line3
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 15);

        v0 = v0 * kernel3[0];
        v1 = v1 * kernel3[1];
        v2 = v2 * kernel3[2];
        v3 = line3 * kernel3[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 3);

    	v4 = v4 * kernel3[4];
        v5 = v5 * kernel3[5];
        v6 = v6 * kernel3[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line4
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 15);

        v0 = v0 * kernel4[0];
        v1 = v1 * kernel4[1];
        v2 = v2 * kernel4[2];
        v3 = line4 * kernel4[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 3);

    	v4 = v4 * kernel4[4];
        v5 = v5 * kernel4[5];
        v6 = v6 * kernel4[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line5
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 15);

        v0 = v0 * kernel5[0];
        v1 = v1 * kernel5[1];
        v2 = v2 * kernel5[2];
        v3 = line5 * kernel5[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 3);

    	v4 = v4 * kernel5[4];
        v5 = v5 * kernel5[5];
        v6 = v6 * kernel5[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line6
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 15);

        v0 = v0 * kernel6[0];
        v1 = v1 * kernel6[1];
        v2 = v2 * kernel6[2];
        v3 = line6 * kernel6[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 3);

    	v4 = v4 * kernel6[4];
        v5 = v5 * kernel6[5];
        v6 = v6 * kernel6[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);
    	
		line0_before = line0;
		line1_before = line1;
		line2_before = line2;
		line3_before = line3;
		line4_before = line4;
		line5_before = line5;
		line6_before = line6;

		line0 = line0_after;
		line1 = line1_after;
		line2 = line2_after;
		line3 = line3_after;
		line4 = line4_after;
		line5 = line5_after;
		line6 = line6_after;

		line0_after = *in0++;
    	line1_after = *in1++;
    	line2_after = *in2++;
    	line3_after = *in3++;
    	line4_after = *in4++;
    	line5_after = *in5++;
    	line6_after = *in6++;

    	*outputLine++ = max;
	}

	if(width & 8)
	{
		uchar16 v0, v1, v2, v3, v4, v5, v6, max;

		//line0
		v0 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line0_before, line0, 15);

        v0 = v0 * kernel0[0];
        v1 = v1 * kernel0[1];
        v2 = v2 * kernel0[2];
        v3 = line0 * kernel0[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(v0, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line0, line0_after, 3);

    	v4 = v4 * kernel0[4];
        v5 = v5 * kernel0[5];
        v6 = v6 * kernel0[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line1
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line1_before, line1, 15);

        v0 = v0 * kernel1[0];
        v1 = v1 * kernel1[1];
        v2 = v2 * kernel1[2];
        v3 = line1 * kernel1[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line1, line1_after, 3);

    	v4 = v4 * kernel1[4];
        v5 = v5 * kernel1[5];
        v6 = v6 * kernel1[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line2
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line2_before, line2, 15);

        v0 = v0 * kernel2[0];
        v1 = v1 * kernel2[1];
        v2 = v2 * kernel2[2];
        v3 = line2 * kernel2[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line2, line2_after, 3);

    	v4 = v4 * kernel2[4];
        v5 = v5 * kernel2[5];
        v6 = v6 * kernel2[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line3
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line3_before, line3, 15);

        v0 = v0 * kernel3[0];
        v1 = v1 * kernel3[1];
        v2 = v2 * kernel3[2];
        v3 = line3 * kernel3[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line3, line3_after, 3);

    	v4 = v4 * kernel3[4];
        v5 = v5 * kernel3[5];
        v6 = v6 * kernel3[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line4
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line4_before, line4, 15);

        v0 = v0 * kernel4[0];
        v1 = v1 * kernel4[1];
        v2 = v2 * kernel4[2];
        v3 = line4 * kernel4[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line4, line4_after, 3);

    	v4 = v4 * kernel4[4];
        v5 = v5 * kernel4[5];
        v6 = v6 * kernel4[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line5
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line5_before, line5, 15);

        v0 = v0 * kernel5[0];
        v1 = v1 * kernel5[1];
        v2 = v2 * kernel5[2];
        v3 = line5 * kernel5[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line5, line5_after, 3);

    	v4 = v4 * kernel5[4];
        v5 = v5 * kernel5[5];
        v6 = v6 * kernel5[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);

    	//line6
    	v0 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 13);
		v1 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 14);
		v2 = __builtin_shave_cmu_alignvec_rri_char16(line6_before, line6, 15);

        v0 = v0 * kernel6[0];
        v1 = v1 * kernel6[1];
        v2 = v2 * kernel6[2];
        v3 = line6 * kernel6[3];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v0);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v1);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v2);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v3);

    	v4 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 1);
    	v5 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 2);
    	v6 = __builtin_shave_cmu_alignvec_rri_char16(line6, line6_after, 3);

    	v4 = v4 * kernel6[4];
        v5 = v5 * kernel6[5];
        v6 = v6 * kernel6[6];
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v4);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v5);
    	max = __builtin_shave_cmu_max_u8_rr_uchar16(max, v6);
        
        uchar8 maxOutput = {max[0], max[1], max[2], max[3], max[4], max[5], max[6], max[7]};
        uchar8 *outputLine8 = (uchar8*)outputLine;
        *outputLine8 = maxOutput;
    	
	}
}
#endif //MOVICOMPILE_OPTIMIZED
