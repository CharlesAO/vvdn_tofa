#include <math.h>
#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/bilateral5x5/bilateral5x5.h>

///This kernels performs a bilateral filter on the input image of 5x5 dimensions
///@param[in]   input   - Input lines, 16-bits unsigned char
///@param[in]   sigma   - Sigma values, 16-bits unsigned char
///@param[in]   width   - Width of input line
///@param[out]  output  - Output lines, 16-bits unsigned char
#ifdef SIPP_USE_MVCV
void mvcvBilateral5x5_asm(UInt16* input, UInt16* sigma, UInt32 width, UInt16* output);
#else
UInt16 bilateralVal(UInt16 *pDepthData, int depthStride, UInt16 sigma);
void mvcvBilateral5x5    (UInt16* input, UInt16* sigma, UInt32 width, UInt16* output)
{
    UInt32 i;

    output[0] = input[0];
    output[1] = input[1];
    output[width - 1] = input[width - 1];
    output[width - 2] = input[width - 2];

    for(i = 2; i < width - 2; i++)
    {
        output[i] = bilateralVal(input + i, width, sigma[i]);
    }
}

UInt16 max(UInt16 a, UInt16 b)
{
    if(a > b)
        return a;
    return b;
}

UInt16 bilateralVal(UInt16 *pDepthData, int depthStride, UInt16 sigma)
{
    int dCenter = (int) pDepthData[0];
    /*if (0 == dCenter)
    {
        return 0;
    }*/

    float sumDepth = 0;
    float sumWeights = 0;
    int wy;
    int wx;

    sigma = max(sigma, (uint16_t)1);
    float sigmaFactor = -0.5f / (float) sigma;

    const UInt16* pData = pDepthData  -2 * depthStride - 2;
    for (wy = -2; wy <= 2; wy++)
    {

        for (wx = 0; wx < 5; wx++)
        {
            int dCurrent = (int) pData[wx];

            float dist = (float) fabs(dCurrent - dCenter);
            float weight = exp(sigmaFactor * dist);

            sumDepth += ((weight * (float) dCurrent));
            sumWeights += weight;
        }
        pData += depthStride;
    }

    return (UInt16) (0.5f + sumDepth / sumWeights);
}
#endif

//UInt16 *sigma;
void svuBilateral5x5(SippFilter *fptr)
{
    UInt16 *input;
    UInt16 *output;
    Bilateral5x5Param *param = (Bilateral5x5Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        input = (UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt16 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvBilateral5x5_asm(input, param->sigma, fptr->sliceWidth, output);
    #else
        mvcvBilateral5x5(input, param->sigma, fptr->sliceWidth, output);
    #endif
    }
}
