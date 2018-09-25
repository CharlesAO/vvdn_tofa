#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/dilateGeneric/dilateGeneric.h>

#ifdef SIPP_USE_MVCV
void mvcvDilate_asm(UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width, UInt32 height, UInt32 k);
#else
void DilateGeneric(UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width, UInt32 height, UInt32 k)
{
    UInt32 j, i1, j1;
    UInt8 max = 0;
    UInt8* row[15];
    height+=1;

    for(j = 0;j < k; j++)
        row[j] = src[j];

    // Determine the anchor positions which must be replaces by a maximum value
    for ( j = 0; j < width; j++)
    {
        max = 0;
        // Determine the maximum number into a k*k square
        for (i1 = 0; i1 < k; i1++ )
        {
            for (j1 = 0; j1 < k; j1++)
            {
                if (kernel[i1][j1] && (max < row[i1][j1 + j]))
                {
                        max = row[i1][j1 + j];
                }
            }
        }
        dst[0][j] = max;
    }
}
#endif

void svuDilateGeneric(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[15];
    DilateGenericParam *param = (DilateGenericParam*)fptr->params;
    UInt32 i, k;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    k = param->kernelSize;
    if (k > 16) k = 16;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        for (i = 0; i < k; i++)
        {
             iline[i]=(UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
        }

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvDilate_asm(iline, &output, (UInt8**)param->dMat, fptr->sliceWidth, 1, param->kernelSize);
    #else
        DilateGeneric(iline, &output, (UInt8**)(param->dMat), fptr->sliceWidth, 1, param->kernelSize);
    #endif
    }
}
