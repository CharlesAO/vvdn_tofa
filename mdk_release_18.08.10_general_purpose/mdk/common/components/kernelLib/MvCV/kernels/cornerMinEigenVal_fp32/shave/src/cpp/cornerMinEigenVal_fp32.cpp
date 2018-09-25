#include "cornerMinEigenVal_fp32.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mvcvCornerMinEigenVal_fp32(u8 **in_lines, fp32 **out_line, fp32 * buffer, u32 width)
{
     fp32 *dx1, *dx2, *dx3;
     fp32 *dy1, *dy2, *dy3;

     fp32 a, b, c, x;
     fp32 *output = *out_line;
     s32 i;
     u8* lines[5];

    //sobel matrix
    float VertSobel[3][3]={
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
    };

    float HorzSobel[3][3]={
        { 1,  2,  1},
        { 0,  0,  0},
        {-1, -2, -1}
    };

    dx1   = (fp32 *)buffer;
    dx2   = (fp32 *)dx1 + width + 8;
    dx3   = (fp32 *)dx2 + width + 8;
    dy1   = (fp32 *)dx3 + width + 8;
    dy2   = (fp32 *)dy1 + width + 8;
    dy3   = (fp32 *)dy2 + width + 8;

    for(i=0;i<5;i++)
    {
        lines[i]=in_lines[i] - 1;
    }

    for (i=0; i<(int)width+2; i++)
    {
        dx1[i]    = VertSobel[0][0]*lines[0][i-1]+ VertSobel[0][1]*lines[0][i-0] +VertSobel[0][2]*lines[0][i+1]+
                    VertSobel[1][0]*lines[1][i-1]+ VertSobel[1][1]*lines[1][i-0] +VertSobel[1][2]*lines[1][i+1]+
                    VertSobel[2][0]*lines[2][i-1]+ VertSobel[2][1]*lines[2][i-0] +VertSobel[2][2]*lines[2][i+1];

        dy1[i]    = HorzSobel[0][0]*lines[0][i-1]+ HorzSobel[0][1]*lines[0][i-0] +HorzSobel[0][2]*lines[0][i+1]+
                    HorzSobel[1][0]*lines[1][i-1]+ HorzSobel[1][1]*lines[1][i-0] +HorzSobel[1][2]*lines[1][i+1]+
                    HorzSobel[2][0]*lines[2][i-1]+ HorzSobel[2][1]*lines[2][i-0] +HorzSobel[2][2]*lines[2][i+1];

        dx2[i]    = VertSobel[0][0]*lines[1][i-1]+ VertSobel[0][1]*lines[1][i-0] +VertSobel[0][2]*lines[1][i+1]+
                    VertSobel[1][0]*lines[2][i-1]+ VertSobel[1][1]*lines[2][i-0] +VertSobel[1][2]*lines[2][i+1]+
                    VertSobel[2][0]*lines[3][i-1]+ VertSobel[2][1]*lines[3][i-0] +VertSobel[2][2]*lines[3][i+1];

        dy2[i]    = HorzSobel[0][0]*lines[1][i-1]+ HorzSobel[0][1]*lines[1][i-0] +HorzSobel[0][2]*lines[1][i+1]+
                    HorzSobel[1][0]*lines[2][i-1]+ HorzSobel[1][1]*lines[2][i-0] +HorzSobel[1][2]*lines[2][i+1]+
                    HorzSobel[2][0]*lines[3][i-1]+ HorzSobel[2][1]*lines[3][i-0] +HorzSobel[2][2]*lines[3][i+1];

        dx3[i]    = VertSobel[0][0]*lines[2][i-1]+ VertSobel[0][1]*lines[2][i-0] +VertSobel[0][2]*lines[2][i+1]+
                    VertSobel[1][0]*lines[3][i-1]+ VertSobel[1][1]*lines[3][i-0] +VertSobel[1][2]*lines[3][i+1]+
                    VertSobel[2][0]*lines[4][i-1]+ VertSobel[2][1]*lines[4][i-0] +VertSobel[2][2]*lines[4][i+1];

        dy3[i]    = HorzSobel[0][0]*lines[2][i-1]+ HorzSobel[0][1]*lines[2][i-0] +HorzSobel[0][2]*lines[2][i+1]+
                    HorzSobel[1][0]*lines[3][i-1]+ HorzSobel[1][1]*lines[3][i-0] +HorzSobel[1][2]*lines[3][i+1]+
                    HorzSobel[2][0]*lines[4][i-1]+ HorzSobel[2][1]*lines[4][i-0] +HorzSobel[2][2]*lines[4][i+1];

    }

    dx1   = dx1   + 1;
    dx2   = dx2   + 1;
    dx3   = dx3   + 1;
    dy1   = dy1   + 1;
    dy2   = dy2   + 1;
    dy3   = dy3   + 1;

    for (i=0; i<(int)width;i++)
    {
        a    = ((dx1[i-1]*dx1[i-1] + dx1[i+0]*dx1[i+0] + dx1[i+1]*dx1[i+1] +
                 dx2[i-1]*dx2[i-1] + dx2[i+0]*dx2[i+0] + dx2[i+1]*dx2[i+1] +
                 dx3[i-1]*dx3[i-1] + dx3[i+0]*dx3[i+0] + dx3[i+1]*dx3[i+1])/9.0f) * 0.5f;
        b    =  (dx1[i-1]*dy1[i-1] + dx1[i+0]*dy1[i+0] + dx1[i+1]*dy1[i+1] +
                 dx2[i-1]*dy2[i-1] + dx2[i+0]*dy2[i+0] + dx2[i+1]*dy2[i+1] +
                 dx3[i-1]*dy3[i-1] + dx3[i+0]*dy3[i+0] + dx3[i+1]*dy3[i+1])/9.0f;
        c    = ((dy1[i-1]*dy1[i-1] + dy1[i+0]*dy1[i+0] + dy1[i+1]*dy1[i+1] +
                 dy2[i-1]*dy2[i-1] + dy2[i+0]*dy2[i+0] + dy2[i+1]*dy2[i+1] +
                 dy3[i-1]*dy3[i-1] + dy3[i+0]*dy3[i+0] + dy3[i+1]*dy3[i+1])/9.0f)* 0.5f;
        x = a + c - sqrt((a - c)*(a - c) + b*b);
        output[i] = x;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvCornerMinEigenVal_fp32_opt(u8 **in_lines, fp32 **out_line, fp32 *buffer, u32 width)
{

    //sobel matrix
     float4 VertSobel[3] = {
        {1.0, 0.0, -1.0},
        {2.0, 0.0, -2.0},
        {1.0, 0.0, -1.0}
    };

     float4 HorzSobel[3] = {
        { 1.0,  2.0,  1.0},
        { 0.0,  0.0,  0.0},
        {-1.0, -2.0, -1.0}
    };


   //Initialize lines pointersVertSobel
    uchar4 *in0 = *(uchar4**)in_lines++;
    uchar4 *in1 = *(uchar4**)in_lines++;
    uchar4 *in2 = *(uchar4**)in_lines++;
    uchar4 *in3 = *(uchar4**)in_lines++;
    uchar4 *in4 = *(uchar4**)in_lines++;
    float4 *outputLine = *(float4**)out_line;


    float4* dx1   = (float4*)((float*)buffer);
    float4* dx2   = (float4*)((float*)dx1 + width + 8);
    float4* dx3   = (float4*)((float*)dx2 + width + 8);
    float4* dy1   = (float4*)((float*)dx3 + width + 8);
    float4* dy2   = (float4*)((float*)dy1 + width + 8);
    float4* dy3   = (float4*)((float*)dy2 + width + 8);

    //Load first 5 lines
    uchar4 line0_before = in0[-1];
    uchar4 line1_before = in1[-1];
    uchar4 line2_before = in2[-1];
    uchar4 line3_before = in3[-1];
    uchar4 line4_before = in4[-1];

    // load the pixels - first round
    uchar4 line0 = *in0++;
    uchar4 line1 = *in1++;
    uchar4 line2 = *in2++;
    uchar4 line3 = *in3++;
    uchar4 line4 = *in4++;

    //convert
    float4 line0_before_fp32 = mvuConvert_float4(line0_before);
    float4 line1_before_fp32 = mvuConvert_float4(line1_before);
    float4 line2_before_fp32 = mvuConvert_float4(line2_before);
    float4 line3_before_fp32 = mvuConvert_float4(line3_before);
    float4 line4_before_fp32 = mvuConvert_float4(line4_before);


    for (u32 i = 0; i < (width / 4 + 1); i++)
    {
        float4 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;
        float4 sum, mul1, mul2, mul3;
    UNUSED(v11);
    UNUSED(v10);
    UNUSED(mul3);
        UNUSED(mul2);
        UNUSED(mul1);
        // do the conversion
        float4 line0_fp32 = mvuConvert_float4(line0);
        float4 line1_fp32 = mvuConvert_float4(line1);
        float4 line2_fp32 = mvuConvert_float4(line2);
        float4 line3_fp32 = mvuConvert_float4(line3);
        float4 line4_fp32 = mvuConvert_float4(line4);

        //load the pixels - next round
        line0 = *in0++;
        line1 = *in1++;
        line2 = *in2++;
        line3 = *in3++;
        line4 = *in4++;

        v0 = __builtin_shave_cmu_alignvec_rri_float4(line0_before_fp32, line0_fp32, 8);
        v1 = __builtin_shave_cmu_alignvec_rri_float4(line1_before_fp32, line1_fp32, 8);
        v2 = __builtin_shave_cmu_alignvec_rri_float4(line2_before_fp32, line2_fp32, 8);

        v5 = __builtin_shave_cmu_alignvec_rri_float4(line0_before_fp32, line0_fp32, 12);
        v6 = __builtin_shave_cmu_alignvec_rri_float4(line1_before_fp32, line1_fp32, 12);
        v7 = __builtin_shave_cmu_alignvec_rri_float4(line2_before_fp32, line2_fp32, 12);

        //dx1
        sum  = line0_fp32 * VertSobel[0][2];
        sum += line1_fp32 * VertSobel[1][2];
        sum += line2_fp32 * VertSobel[2][2];
        sum += v0 * VertSobel[0][0];
        sum += v1 * VertSobel[1][0];
        sum += v2 * VertSobel[2][0];
        sum += v5 * VertSobel[0][1];
        sum += v6 * VertSobel[1][1];
        sum += v7 * VertSobel[2][1];
        dx1[i] = sum;

        //dy1
        sum  = line0_fp32 * HorzSobel[0][2];
        sum += line1_fp32 * HorzSobel[1][2];
        sum += line2_fp32 * HorzSobel[2][2];
        sum += v0 * HorzSobel[0][0];
        sum += v1 * HorzSobel[1][0];
        sum += v2 * HorzSobel[2][0];
        sum += v5 * HorzSobel[0][1];
        sum += v6 * HorzSobel[1][1];
        sum += v7 * HorzSobel[2][1];
        dy1[i] = sum;

        v3 = __builtin_shave_cmu_alignvec_rri_float4(line3_before_fp32, line3_fp32, 8);
        v8 = __builtin_shave_cmu_alignvec_rri_float4(line3_before_fp32, line3_fp32, 12);

        //dx2
        sum  = line1_fp32 * VertSobel[0][2];
        sum += line2_fp32 * VertSobel[1][2];
        sum += line3_fp32 * VertSobel[2][2];
        sum += v1 * VertSobel[0][0];
        sum += v2 * VertSobel[1][0];
        sum += v3 * VertSobel[2][0];
        sum += v6 * VertSobel[0][1];
        sum += v7 * VertSobel[1][1];
        sum += v8 * VertSobel[2][1];
        dx2[i] = sum;

        //dy2
        sum  = line1_fp32 * HorzSobel[0][2];
        sum += line2_fp32 * HorzSobel[1][2];
        sum += line3_fp32 * HorzSobel[2][2];
        sum += v1 * HorzSobel[0][0];
        sum += v2 * HorzSobel[1][0];
        sum += v3 * HorzSobel[2][0];
        sum += v6 * HorzSobel[0][1];
        sum += v7 * HorzSobel[1][1];
        sum += v8 * HorzSobel[2][1];
        dy2[i] = sum;

        v4 = __builtin_shave_cmu_alignvec_rri_float4(line4_before_fp32, line4_fp32, 8);
        v9 = __builtin_shave_cmu_alignvec_rri_float4(line4_before_fp32, line4_fp32, 12);

        //dx3
        sum  = line2_fp32 * VertSobel[0][2];
        sum += line3_fp32 * VertSobel[1][2];
        sum += line4_fp32 * VertSobel[2][2];
        sum += v2 * VertSobel[0][0];
        sum += v3 * VertSobel[1][0];
        sum += v4 * VertSobel[2][0];
        sum += v7 * VertSobel[0][1];
        sum += v8 * VertSobel[1][1];
        sum += v9 * VertSobel[2][1];
        dx3[i] = sum;

        //dy3
        sum  = line2_fp32 * HorzSobel[0][2];
        sum += line3_fp32 * HorzSobel[1][2];
        sum += line4_fp32 * HorzSobel[2][2];
        sum += v2 * HorzSobel[0][0];
        sum += v3 * HorzSobel[1][0];
        sum += v4 * HorzSobel[2][0];
        sum += v7 * HorzSobel[0][1];
        sum += v8 * HorzSobel[1][1];
        sum += v9 * HorzSobel[2][1];
        dy3[i] = sum;

        //copy current -> before
        line0_before_fp32 = line0_fp32;
        line1_before_fp32 = line1_fp32;
        line2_before_fp32 = line2_fp32;
        line3_before_fp32 = line3_fp32;
        line4_before_fp32 = line4_fp32;

    }

    float4 a, b, c, x, xRoot;
    UNUSED(xRoot);
    float4 dx1Vec, dx1Vec_after, dx2Vec, dx2Vec_after, dx3Vec, dx3Vec_after;
    float4 dx1VecSquare, dx1VecSquare_after, dx2VecSquare, dx2VecSquare_after, dx3VecSquare, dx3VecSquare_after;
    float4 dy1Vec, dy1Vec_after, dy2Vec, dy2Vec_after, dy3Vec, dy3Vec_after;
    float4 dy1VecSquare, dy1VecSquare_after, dy2VecSquare, dy2VecSquare_after, dy3VecSquare, dy3VecSquare_after;


    for (u32 i = 0; i < (width/4); i++)
    {
        float4 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;

        dx1Vec = *dx1++;
        dx2Vec = *dx2++;
        dx3Vec = *dx3++;
        dx1Vec_after = *dx1;
        dx2Vec_after = *dx2;
        dx3Vec_after = *dx3;

        dy1Vec = *dy1++;
        dy2Vec = *dy2++;
        dy3Vec = *dy3++;
        dy1Vec_after = *dy1;
        dy2Vec_after = *dy2;
        dy3Vec_after = *dy3;

        dx1VecSquare = dx1Vec * dx1Vec;
        dx1VecSquare_after = dx1Vec_after * dx1Vec_after;

        dx2VecSquare = dx2Vec * dx2Vec;
        dx2VecSquare_after = dx2Vec_after * dx2Vec_after;

        dx3VecSquare = dx3Vec * dx3Vec;
        dx3VecSquare_after = dx3Vec_after * dx3Vec_after;

        a = dx1VecSquare + dx2VecSquare;
        a += dx3VecSquare;

        v0 = __builtin_shave_cmu_alignvec_rri_float4(dx1VecSquare, dx1VecSquare_after, 4);
        a += v0;
        v1 = __builtin_shave_cmu_alignvec_rri_float4(dx1VecSquare, dx1VecSquare_after, 8);
        a += v1;
        v2 = __builtin_shave_cmu_alignvec_rri_float4(dx2VecSquare, dx2VecSquare_after, 4);
        a += v2;
        v3 = __builtin_shave_cmu_alignvec_rri_float4(dx2VecSquare, dx2VecSquare_after, 8);
        a += v3;
        v4 = __builtin_shave_cmu_alignvec_rri_float4(dx3VecSquare, dx3VecSquare_after, 4);
        a += v4;
        v5 = __builtin_shave_cmu_alignvec_rri_float4(dx3VecSquare, dx3VecSquare_after, 8);
        a += v5;
        a = a * 0.05555555;

        v6 = dy1Vec * dx1Vec;
        v7 = dy1Vec_after * dx1Vec_after;
        dy1VecSquare = dy1Vec * dy1Vec;
        dy1VecSquare_after = dy1Vec_after * dy1Vec_after;

        v8 = dy2Vec * dx2Vec;
        v9 = dy2Vec_after * dx2Vec_after;
        dy2VecSquare = dy2Vec * dy2Vec;
        dy2VecSquare_after = dy2Vec_after * dy2Vec_after;

        v10 = dy3Vec * dx3Vec;
        v11 = dy3Vec_after * dx3Vec_after;
        dy3VecSquare = dy3Vec * dy3Vec;
        dy3VecSquare_after = dy3Vec_after * dy3Vec_after;

        b = v6 + v8;
        b += v10;

        v0 = __builtin_shave_cmu_alignvec_rri_float4(v6, v7, 4);
        b += v0;
        v1 = __builtin_shave_cmu_alignvec_rri_float4(v6, v7, 8);
        b += v1;
        v2 = __builtin_shave_cmu_alignvec_rri_float4(v8, v9, 4);
        b += v2;
        v3 = __builtin_shave_cmu_alignvec_rri_float4(v8, v9, 8);
        b += v3;
        v4 = __builtin_shave_cmu_alignvec_rri_float4(v10, v11, 4);
        b += v4;
        v5 = __builtin_shave_cmu_alignvec_rri_float4(v10, v11, 8);
        b += v5;
        b = b * 0.11111111;

        c = dy1VecSquare + dy2VecSquare;
        c += dy3VecSquare;

        v0 = __builtin_shave_cmu_alignvec_rri_float4(dy1VecSquare, dy1VecSquare_after, 4);
        c += v0;
        v1 = __builtin_shave_cmu_alignvec_rri_float4(dy1VecSquare, dy1VecSquare_after, 8);
        c += v1;
        v2 = __builtin_shave_cmu_alignvec_rri_float4(dy2VecSquare, dy2VecSquare_after, 4);
        c += v2;
        v3 = __builtin_shave_cmu_alignvec_rri_float4(dy2VecSquare, dy2VecSquare_after, 8);
        c += v3;
        v4 = __builtin_shave_cmu_alignvec_rri_float4(dy3VecSquare, dy3VecSquare_after, 4);
        c += v4;
        v5 = __builtin_shave_cmu_alignvec_rri_float4(dy3VecSquare, dy3VecSquare_after, 8);
        c += v5;
        c = c * 0.05555555;

        float4 outputFloat, temp, tempAC;
        tempAC = a - c;
        x = a + c;
        temp = tempAC * tempAC + b * b;
        temp[0] = sqrtf(temp[0]);
        temp[1] = sqrtf(temp[1]);
        temp[2] = sqrtf(temp[2]);
        temp[3] = sqrtf(temp[3]);

        outputFloat = x - temp;
        *outputLine++ = mvuConvert_float4(outputFloat);
    }

}
#endif //MOVICOMPILE_OPTIMIZED