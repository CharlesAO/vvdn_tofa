#include <sipp.h>
#include <sippShaveMacros.h>

//#############################################################################
/// scale05BilinHV_U16ToU16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] inWidth  - width of input line

#if defined(SIPP_USE_MVCV)
void mvispScale05BilinHV_U8ToU8_asm  (UInt8  **input, UInt8 *output,  int width);
void mvispScale05BilinHV_U16ToU16_asm(UInt16 **input, UInt16 *output, int width);
#else
void scale05BilinHV_U16ToU16(UInt16 **input, UInt16 *output, int width)
{
    UInt32 i;

    for (i=0; i < width; i++)
    {
        output[i] = ( ((input[0][i*2] + input[0][i*2+1] + 1)>>1) +
            ((input[1][i*2] + input[1][i*2+1] + 1)>>1 ) + 1) >> 1;
    }
}

/// scale05BilinHV_U8ToU8 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] inWidth      - width of input line
void scale05BilinHV_U8ToU8(UInt8 **input, UInt8 *output, int width)
{
    UInt32 i;

    for (i=0; i < width; i++)
    {
    output[i] =(input[0][i*2] + input[0][i*2+1] +
        input[1][i*2] + input[1][i*2+1]) >> 2;
    }
}
#endif


//#############################################################################
void svuScl05BilinHV(SippFilter *fptr)
{
    UInt32  p;

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    UInt32 bpp = sippFilterGetOutputBpp(fptr);
    #else
    UInt32 bpp = sippFilterGetOutputBpp(fptr,0);
    #endif

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        switch (bpp)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 0:  //8bit format
            {
                UInt8 *in[2];
                UInt8 *out;

                //The low level implementation
                UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
                for (p=0; p<numPlanes; p++)
                {
                    out = (UInt8*)getPlaneIoPtrs(fptr, 0, iterNum, p, in);

    #ifdef SIPP_USE_MVCV
                    mvispScale05BilinHV_U8ToU8_asm(in, out, fptr->sliceWidth);
    #else
                    scale05BilinHV_U8ToU8         (in, out, fptr->sliceWidth);
    #endif
                }
                break;
            }

            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 1:  //16bit format
            {
                UInt16 *in[2];
                UInt16 *out;

                //The low level implementation
                UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
                for (p=0; p<numPlanes; p++)
                {
                    out = (UInt16*)getPlaneIoPtrs(fptr, 0, iterNum, p, in);

    #ifdef SIPP_USE_MVCV
                    mvispScale05BilinHV_U16ToU16_asm(in, out, fptr->sliceWidth);
    #else
                    scale05BilinHV_U16ToU16         (in, out, fptr->sliceWidth);
    #endif
                }

                break;
            }

            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        default: //other modes are not supported !
            break;
        }
    }
}
