#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/positionKernel/positionKernel.h>

/// pixel Position kernel      - returns the position of a given pixel value
/// @param[in] srcAddr         - array of pointers to input lines
/// @param[in] maskAddr        - mask filled with 1s and 0s which determines the image area to find position
/// @param[in] width           - line`s width(length)
/// @param[in] pixelValue      - stores the pixel value to be searched
/// @param[out] pixelPosition  - stores the position occupied by the searched value within line
/// @param[out] status         - stores 0x11 if pixel value found, else 0x00
/// @return                    - Nothing

#ifdef SIPP_USE_MVCV
void mvcvPixelPos_asm(UInt8** srcAddr, UInt8* maskAddr, UInt32 width, UInt8 pixelValue, UInt32* pixelPosition, UInt8* status);
#else
void pixelPos        (UInt8** srcAddr, UInt8* maskAddr, UInt32 width, UInt8 pixelValue, UInt32* pixelPosition, UInt8* status)
{
    int j;
    UInt8* src;
    UInt8* mask;
    src = srcAddr[0];
    UInt32 location = 0;
    *status = 0;

    for(j = width-1; j >= 0; j--)
    {
        if (maskAddr[j] != 0)
        {
            if(src[j] == pixelValue)
            {
                location = j;
                *status = 0x11;
            }
        }
    }
    *pixelPosition = location ;
}
#endif

void svuPositionKernel(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];
    UInt32 i;

    positionKernelParam *param = (positionKernelParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        for(i=0; i < fptr->sliceWidth; i++)
            output[i] = 0x00;

        param->pixelPosition = 0;
        param->status = 0;

    #ifdef SIPP_USE_MVCV
        mvcvPixelPos_asm(iline, param->maskAddr, fptr->sliceWidth, param->pixelValue, &param->pixelPosition, &param->status);
                         output[0] = param->status; output[1] = param->pixelPosition;
    #else
        pixelPos(iline, param->maskAddr, fptr->sliceWidth, param->pixelValue, &param->pixelPosition, &param->status);
                 output[0] = param->status; output[1] = param->pixelPosition;
    #endif
    }
}