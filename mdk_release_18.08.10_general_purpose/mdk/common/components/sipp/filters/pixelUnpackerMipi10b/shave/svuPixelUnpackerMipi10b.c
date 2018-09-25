#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/pixelUnpackerMipi10b/pixelUnpackerMipi10b.h>

/// pixelUnpackerMipi10b
/// input format 32b: xxxxxxxxxxxxa0a1a2a3a4a5a6a7a8a9b0b1b2b3b4b5b6b7b8b9
/// output format 16b:  000000a0a1a2a3a4a5a6a7a8a9 000000b0b1b2b3b4b5b6b7b8b9

/// @param[in]  in      - pointer to input line
/// @param[out] out     - pointer to output line
/// @param[in] width    - number of processed pixels
/// @param[in] lineNo   - line number
/// @param[in] corfs    - coefficients for black correction

#ifdef SIPP_USE_MVCV
void __attribute__((overloadable)) mvispPixelUnpackerMipi10b_asm(UInt32 *in, UInt16 *out, UInt32 width, UInt32 lineNo, UInt32 coefs[4], int noMipiRxWorkaround);
#else
void pixelUnpackerMipi10b                                       (UInt32 *in, UInt16 *out, UInt32 width, UInt32 lineNo, UInt32 coefs[4], int noMipiRxWorkaround)
{
    UInt32 mask1;
    UInt32 mask2;
    int i, shift;
    int j = 0;
    UInt32 coef1, coef2;
    double out1, out2;

    if (noMipiRxWorkaround) {
        mask1 = 0x0000FFFF;
        mask2 = 0xFFFF0000;
        shift = 16;
    } else {
        mask1 = 0x000003FF;
        mask2 = 0x000FFC00;
        shift = 10;
    }

    if(lineNo % 2 == 0)
    {
        coef1 = coefs[0];
        coef2 = coefs[1];
    }
    else
    {
        coef1 = coefs[2];
        coef2 = coefs[3];
    }

    for(i = 0; i < width; i = i + 2)
    {
        out1 = (double)((in[j] & mask1)) - (double)coef2;
        out2 = (double)((in[j++] & mask2) >> shift) - (double)coef1;

        if (out1 >= 65535)
            out1 = 65535;
        if (out1 <= 0)
            out1 = 0;

        if (out2 >= 65535)
            out2 = 65535;
        if (out2 <= 0)
            out2 = 0;

        out[i] = (UInt16)(out1);
        out[i + 1] = (UInt16)(out2);
    }
}
#endif

void svuPixelUnpackerMipi10b(SippFilter *fptr)
{
    UInt32 *input;
    UInt16 *output;
    PixelUnpackerMipi10bParam *param = (PixelUnpackerMipi10bParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi   = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //input line
        input = (UInt32*)getInPtr(fptr,0,iterNum,0,0);

        //output line
        output = (UInt16*)getOutPtr(fptr, iterNum,0);

    #ifdef SIPP_USE_MVCV
        mvispPixelUnpackerMipi10b_asm(input, output, fptr->sliceWidth, runNo, param->coefs, param->noMipiRxWorkaround);
    #else
        pixelUnpackerMipi10b(input, output, fptr->sliceWidth, runNo, param->coefs, param->noMipiRxWorkaround);
    #endif
    }
}
