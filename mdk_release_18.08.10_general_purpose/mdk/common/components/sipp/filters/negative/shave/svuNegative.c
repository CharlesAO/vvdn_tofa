///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/negative/negative.h>

void negativeFilterImplementation(int* inLine, UInt8* outLine, int widthLine )
{
#if defined(__sparc) && !defined(MYRIAD2)
    int4 inval;
    int4 rez;
#else
    int inval[4];
    int rez[4];
#endif
    int i = 0, j = 0;

    while (j < widthLine)
    {
        inval[0] = (int) (inLine[i + 0]);
        inval[1] = (int) (inLine[i + 1]);
        inval[2] = (int) (inLine[i + 2]);
        inval[3] = (int) (inLine[i + 3]);

        rez[0] = 0xffffffff - inval[0];
        rez[1] = 0xffffffff - inval[1];
        rez[2] = 0xffffffff - inval[2];
        rez[3] = 0xffffffff - inval[3];

        outLine[j +  0] = (UInt8) (rez[0] >>  0) & 0xFF;
        outLine[j +  2] = (UInt8) (rez[0] >> 16) & 0xFF;
        outLine[j +  1] = (UInt8) (rez[0] >>  8) & 0xFF;
        outLine[j +  3] = (UInt8) (rez[0] >> 24) & 0xFF;
        outLine[j +  4] = (UInt8) (rez[1] >>  0) & 0xFF;
        outLine[j +  5] = (UInt8) (rez[1] >>  8) & 0xFF;
        outLine[j +  6] = (UInt8) (rez[1] >> 16) & 0xFF;
        outLine[j +  7] = (UInt8) (rez[1] >> 24) & 0xFF;
        outLine[j +  8] = (UInt8) (rez[2] >>  0) & 0xFF;
        outLine[j +  9] = (UInt8) (rez[2] >>  8) & 0xFF;
        outLine[j + 10] = (UInt8) (rez[2] >> 16) & 0xFF;
        outLine[j + 11] = (UInt8) (rez[2] >> 24) & 0xFF;
        outLine[j + 12] = (UInt8) (rez[3] >>  0) & 0xFF;
        outLine[j + 13] = (UInt8) (rez[3] >>  8) & 0xFF;
        outLine[j + 14] = (UInt8) (rez[3] >> 16) & 0xFF;
        outLine[j + 15] = (UInt8) (rez[3] >> 24) & 0xFF;
        j += 16;
        i += 4;
    }
}

void svuNegative(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 1 input line
        iline = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #if 0
        //copy-paste
        for (i = 0; i < fptr->sliceWidth; i++)
            output[i] = iline[1][i];
    #else
        negativeFilterImplementation((Int32*)iline, output, fptr->sliceWidth);
    #endif
    }
}
