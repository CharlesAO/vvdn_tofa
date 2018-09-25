#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv3x3/conv3x3.h>

/// Convolution 3x3 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

void conv3x3FilterImplementation (UInt8 *inLine[3], UInt8 *outLine, half *f, UInt32 widthLine)
{
    UInt32 x;

    for (x=0; x<widthLine; x++)
    {
        float v00 = (float)inLine[0][x-1];
        float v01 = (float)inLine[0][x  ];
        float v02 = (float)inLine[0][x+1];

        float v10 = (float)inLine[1][x-1];
        float v11 = (float)inLine[1][x  ];
        float v12 = (float)inLine[1][x+1];

        float v20 = (float)inLine[2][x-1];
        float v21 = (float)inLine[2][x  ];
        float v22 = (float)inLine[2][x+1];


        float vout =  f[0]*v00 + f[1]*v01 + f[2]*v02 +
                      f[3]*v10 + f[4]*v11 + f[5]*v12 +
                      f[6]*v20 + f[7]*v21 + f[8]*v22;

        outLine[x] = clampU8(vout);
    }
}

//##########################################################################################
void svuConv3x3(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[3];
    UInt32  plane;
    Conv3x3Param *param = (Conv3x3Param*)fptr->params;
    UInt32 numPlanes;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane < numPlanes; plane++)
        {
            //Faster way of getting IO pointers
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            void mvcvConvolution3x3_asm(UInt8** in, UInt8** out, half conv[9], UInt32 inWidth);
            mvcvConvolution3x3_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    /*#elif defined(__myriad2__)
           void conv3x3opt(UInt8 *ln0, UInt8 *ln1, UInt8 *ln2, UInt8 *output, half *mat, UInt32 width);
            conv3x3opt(iline[0], iline[1], iline[2], output, (half*)param->cMat, fptr->sliceWidth);*/
    #else
            conv3x3FilterImplementation(iline, output, (half*)param->cMat, fptr->sliceWidth);
    #endif
       }
    }
}
