#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/mixMedian/mixMedian.h>

//#######################################################################################
/// mix median - Mix two 8-bit planes according to a third 8-bit reference plane
/// @param[out]        - first unsigned int contain number of valid points in the line, after score for every point
/// @param[in] in0       - array of pointers to input line of the image, 3 planes
/// @param[in] in1       - array of pointers to input line of the image, 3 planes
/// @param[in] ref       - array of pointers to input line of the image, 1 planes, reference
/// @param[in] offset    - offset
/// @param[in] slope     - slope
/// @param[in] width     - width of the input lines
/// @return    Nothing

//Octave ref
//      alpha = double(dns_ref)/255;
//      alpha = (alpha + cfg.chroma_median_offset) * cfg.chroma_median_slope;
//      alpha(alpha>1) = 1;
//      alpha(alpha<0) = 0;
//
//      for i = 1:3
//          ci(:,:,i) = alpha .* (ci(:,:,i) - cim(:,:,i)) + cim(:,:,i);
//      end

// Mix two 8-bit planes according to a third 8-bit reference plane

#if defined(SIPP_USE_MVCV)
void mvispMixMedian_asm(UInt8 *out[3], UInt8 *in0[3], UInt8 *in1[3], UInt8 *ref, float offset, float slope, unsigned int width);
#else
void mixMedian         (UInt8 *out[3], UInt8 *in0[3], UInt8 *in1[3], UInt8 *ref, float offset, float slope, unsigned int width)
{
    unsigned int p = 0;
    unsigned int i = 0;
    for (p = 0; p < 3; p++)
    {
        for(i = 0; i < width; i++)
        {
            float alpha;
            alpha = ((float)ref[i] / 255.0f + offset) * slope;;
            alpha = alpha > 1.0f ? 1.0f : alpha;
            alpha = alpha < 0.0f ? 0.0f : alpha;
            out[0][i] = clampU8(alpha * (in0[0][i] - in1[0][i]) + in1[0][i]);
            out[1][i] = clampU8(alpha * (in0[1][i] - in1[1][i]) + in1[1][i]);
            out[2][i] = clampU8(alpha * (in0[2][i] - in1[2][i]) + in1[2][i]);
        }
    }
}
#endif

//#######################################################################################
void svuMixMedian(SippFilter *fptr)
{
    float  slope  = ((MixMedianParam*)fptr->params)->slope;
    float  offset = ((MixMedianParam*)fptr->params)->offset;
    UInt8 *in0[3];
    UInt8 *in1[3];
    UInt8 *ref;
    UInt8 *out[3];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        // Reference to octave code: in0 = ci, in1 = cim, ref = dns_ref
        getIn3PlanePtr(fptr, 0, iterNum, 0, in0);
        getIn3PlanePtr(fptr, 1, iterNum, 0, in1);

        ref = (UInt8 *)getInPtr(fptr, 2, iterNum, 0, 0);

        out[0] = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        out[1] = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        out[2] = (UInt8 *)getOutPtr(fptr, iterNum, 2);

    #ifdef SIPP_USE_MVCV
        mvispMixMedian_asm(out, in0, in1, ref, offset, slope, fptr->sliceWidth);
    #else
        mixMedian         (out, in0, in1, ref, offset, slope, fptr->sliceWidth);
    #endif
    }
}