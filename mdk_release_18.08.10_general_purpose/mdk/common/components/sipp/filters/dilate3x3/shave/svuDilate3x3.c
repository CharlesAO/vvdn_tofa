#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/dilate3x3/dilate3x3.h>

/// Dilate3x3 kernel
/// @param[in]  src      - array of pointers to input lines of the input image
/// @param[out] dst      - array of pointers to output lines
/// @param[in]  kernel   - array of pointers to input kernel
/// @param[in]  width    - width  of the input line

#ifdef SIPP_USE_MVCV
void mvcvDilate3x3_asm(UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width);
#else
void Dilate3x3        (UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width)
{
    int j, i1, j1;
    UInt8 max = 0;
    UInt8* row[3];

    for(j = 0;j < 3; j++)
        row[j] = src[j];

    // Determine the anchor positions which must be replaces by a maximum value
    for ( j = 0; j < width; j++)
    {
        max = 0;
        // Determine the maximum number into a k*k square
        for(i1 = 0; i1 < 3; i1++ )
        {
            for (j1 = 0; j1 < 3; j1++)
            {
                if(kernel[i1][j1]==1)
                {
                    if(max < row[i1][j1 + j - 1])
                    {
                        max = row[i1][j1 + j - 1];
                    }
                }
            }
        }
        dst[0][j] =  max;
    }
}
#endif

void svuDilate3x3(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[3];
    Dilate3x3Param *param = (Dilate3x3Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvDilate3x3_asm(iline, &output, (UInt8**)(param->dMat), fptr->sliceWidth);
    #else
        Dilate3x3(iline, &output, (UInt8**)(param->dMat), fptr->sliceWidth);
    #endif
    }
}
