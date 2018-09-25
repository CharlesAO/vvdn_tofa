#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBtoChromaNV12/cvtColorRGBtoChromaNV12.h>

//#######################################################################################
/// cvtColorKernel to conversion RGB to chromaNV12
///@param[in] inR input R channel
///@param[in] inG input G channel
///@param[in] inB input B channel
///@param[out] uvOut output UV interleaved channel

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBtoChromaNV12_asm(UInt8* ln1RGB[3], UInt8* ln2RGB[3],  UInt8* vuOut, float* coefsMat, float* offset, UInt32 width);
#else
void cvtColorRGBtoChromaNV12        (UInt8* ln1RGB[3], UInt8* ln2RGB[3],  UInt8* vuOut, float* coefsMat, float* offset, UInt32 width)
{
    UInt32 i;
    float u, v;
    UInt32 vu_idx = 0;

    for (i = 0; i < width; i+=2)
    {
        // u = (((float)(ln1RGB[0][i] + ln1RGB[0][i + 1] + ln2RGB[0][i] + ln2RGB[0][i + 1]) * 0.25f)/255.0f * coefsMat[3]  +
             // ((float)(ln1RGB[1][i] + ln1RGB[1][i + 1] + ln2RGB[1][i] + ln2RGB[1][i + 1]) * 0.25f)/255.0f * coefsMat[4]  +
             // ((float)(ln1RGB[2][i] + ln1RGB[2][i + 1] + ln2RGB[2][i] + ln2RGB[2][i + 1]) * 0.25f)/255.0f * coefsMat[5]) + offset[1];
        // u = u * 255;
        // v = (((float)(ln1RGB[0][i] + ln1RGB[0][i + 1] + ln2RGB[0][i] + ln2RGB[0][i + 1]) * 0.25f)/255.0f * coefsMat[6]  +
             // ((float)(ln1RGB[1][i] + ln1RGB[1][i + 1] + ln2RGB[1][i] + ln2RGB[1][i + 1]) * 0.25f)/255.0f * coefsMat[7]  +
             // ((float)(ln1RGB[2][i] + ln1RGB[2][i + 1] + ln2RGB[2][i] + ln2RGB[2][i + 1]) * 0.25f)/255.0f * coefsMat[8]) + offset[2];
        // v = v * 255;

        u = (((((ln1RGB[0][i] + ln1RGB[0][i + 1])>>1) + ((ln2RGB[0][i] + ln2RGB[0][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[3]  +
            ((((ln1RGB[1][i] + ln1RGB[1][i + 1])>>1) + ((ln2RGB[1][i] + ln2RGB[1][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[4]  +
            ((((ln1RGB[2][i] + ln1RGB[2][i + 1])>>1) + ((ln2RGB[2][i] + ln2RGB[2][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[5]) + offset[1];
        u = u * 255;
        v = (((((ln1RGB[0][i] + ln1RGB[0][i + 1])>>1) + ((ln2RGB[0][i] + ln2RGB[0][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[6]  +
            ((((ln1RGB[1][i] + ln1RGB[1][i + 1])>>1) + ((ln2RGB[1][i] + ln2RGB[1][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[7]  +
            ((((ln1RGB[2][i] + ln1RGB[2][i + 1])>>1) + ((ln2RGB[2][i] + ln2RGB[2][i + 1])>>1) + 1) >> 1)/255.0f * coefsMat[8]) + offset[2];
        v = v * 255;

        vuOut[vu_idx    ] = (UInt8) (u > 255 ? 255 : u < 0 ? 0 : u);
        vuOut[vu_idx + 1] = (UInt8) (v > 255 ? 255 : v < 0 ? 0 : v);
        vu_idx = vu_idx + 2;

    }
}
#endif

//#######################################################################################
void svuCvtColorRGBtoChromaNV12(SippFilter *fptr)
{
    UInt8 *ln1RGB[3];
    UInt8 *ln2RGB[3];
    UInt8 *outputVU;
    cvtColorChromaNV12Param *param = (cvtColorChromaNV12Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //faster ptr access
        getIn3PlanePtr(fptr, 0, iterNum, 0, ln1RGB);
        getIn3PlanePtr(fptr, 0, iterNum, 1, ln2RGB);

        //the output line
        outputVU = (UInt8 *)getOutPtr(fptr, iterNum,0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBtoChromaNV12_asm(ln1RGB, ln2RGB, outputVU, param->coefsMat, param->offset, fptr->sliceWidth);
    #else
        cvtColorRGBtoChromaNV12        (ln1RGB, ln2RGB, outputVU, param->coefsMat, param->offset, fptr->sliceWidth);
    #endif
    }
}