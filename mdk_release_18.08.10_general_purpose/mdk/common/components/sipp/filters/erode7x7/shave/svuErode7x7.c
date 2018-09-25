#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/erode7x7/erode7x7.h>

/// Erode7x7 kernel
/// @param[in]  src      - array of pointers to input lines of the input image
/// @param[out] dst      - array of pointers to output lines
/// @param[in]  kernel   - array of pointers to input kernel
/// @param[in]  width    - width  of the input line

#ifdef SIPP_USE_MVCV
void mvcvErode7x7_asm(UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width);
#else
void Erode7x7        (UInt8** src, UInt8** dst, UInt8** kernel, UInt32 width)
{
    int j, i1, j1;
    UInt8 min = 0xFF;
    UInt8* row[7];

    for(j = 0;j < 7; j++)
        row[j] = src[j];

    // Determine the anchor positions which must be replaces by a maximum value
    for ( j = 0; j < width; j++)
    {
        min = 0xFF;
        // Determine the minimum number into a k*k square
        for(i1 = 0; i1 < 7; i1++ )
        {
            for (j1 = 0; j1 < 7; j1++)
            {
                if(kernel[i1][j1] == 1)
                {
                    if(min > row[i1][j1 + j - 3])
                    {
                        min = row[i1][j1 + j - 3];
                    }
                }
            }
        }
        dst[0][j] = min;
    }
}
#endif

void svuErode7x7(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[7];
    Erode7x7Param *param = (Erode7x7Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
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
        mvcvErode7x7_asm(iline, &output, (UInt8**)(param->eMat), fptr->sliceWidth);
    #else
        Erode7x7(iline, &output, (UInt8**)(param->eMat), fptr->sliceWidth);
    #endif
    }
}
