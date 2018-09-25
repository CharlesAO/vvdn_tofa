#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/dilate7x7/dilate7x7.h>

#ifdef SIPP_USE_MVCV
void mvcvDilate7x7_asm(UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width);
#else
void Dilate7x7        (UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width)
{
    int j, i1, j1;
    UInt8 max = 0;
    UInt8* row[7];

    for(j = 0;j < 7; j++)
        row[j] = src[j];

    // Determine the anchor positions which must be replaces by a maximum value
    for ( j = 0; j < width; j++)
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
#endif

void svuDilate7x7(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[7];
    Dilate7x7Param *param = (Dilate7x7Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5]=(UInt8 *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6]=(UInt8 *)getInPtr(fptr, 0, iterNum, 6, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvDilate7x7_asm(iline, &output, (UInt8**)(param->dMat), fptr->sliceWidth);
    #else
        Dilate7x7(iline, &output, (UInt8**)(param->dMat), fptr->sliceWidth);
    #endif
    }
}
