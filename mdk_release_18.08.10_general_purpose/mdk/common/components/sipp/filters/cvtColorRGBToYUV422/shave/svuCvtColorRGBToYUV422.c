#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBToYUV422/cvtColorRGBToYUV422.h>

/// Performs color space conversion RGB to YUV422 for one line
/// @param[in] rIn      - pointer to the input line that contain R values from RGB
/// @param[in] gIn      - pointer to the input line that contain G values from RGB
/// @param[in] bIn      - pointer to the input line that contain B values from RGB
/// @param[out] output  - pointer to the output line YUV422 interleaved
/// @param[in] width    - line width

#ifdef SIPP_USE_MVCV
void mvcvCvtColorKernelRGBToYUV422_asm(UInt8** rIn, UInt8** gIn, UInt8** bIn, UInt8** output, UInt32 width);
#else
void cvtColorKernelRGBToYUV422        (UInt8** rIn, UInt8** gIn, UInt8** bIn, UInt8** output, UInt32 width)
{
    int j;
    float y, u1, v1, u2, v2, u_avr, v_avr;
    int out_index = 0;
    UInt8* inR = rIn[0];
    UInt8* inG = gIn[0];
    UInt8* inB = bIn[0];
    UInt8* out_yuyv = output[0];

    for(j = 0; j < width; j+=2)
    {
        y = 0.299f * inR[j] + 0.587f * inG[j] + 0.114f * inB[j];
        u1 = ((float)inB[j] - y) * 0.564f + 128;
        v1 = ((float)inR[j] - y) * 0.713f + 128;

        y = y>255 ? 255 : y<0 ? 0 : y;
        out_yuyv[out_index++] =(UInt8) y;

        y = 0.299f * inR[j + 1] + 0.587f * inG[j + 1] + 0.114f * inB[j + 1];
        y = y>255 ? 255 : y<0 ? 0 : y;
        u2 = ((float)inB[j + 1] - y) * 0.564f + 128;
        v2 = ((float)inR[j + 1] - y) * 0.713f + 128;

        u_avr = (u1 + u2) / 2;
        v_avr = (v1 + v2) / 2;

        out_yuyv[out_index++] =(UInt8) (u_avr>255 ? 255 : u_avr<0 ? 0 : u_avr);
        out_yuyv[out_index++] =(UInt8) y;
        out_yuyv[out_index++] =(UInt8) (v_avr>255 ? 255 : v_avr<0 ? 0 : v_avr);
    }
}
#endif

void svuCvtColorRGBToYUV422(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorKernelRGBToYUV422_asm(&inputR, &inputG, &inputB, &output, fptr->sliceWidth);
    #else
        cvtColorKernelRGBToYUV422(&inputR, &inputG, &inputB, &output, fptr->sliceWidth);
    #endif
    }
}