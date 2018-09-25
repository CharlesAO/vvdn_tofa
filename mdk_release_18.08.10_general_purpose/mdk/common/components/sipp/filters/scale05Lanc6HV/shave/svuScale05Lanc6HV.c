#include <sipp.h>
#include <sippShaveMacros.h>

#ifdef SIPP_USE_MVCV
void mvispScale05Lanc6HV_asm(UInt8 *in[6], UInt8 *out,unsigned int width);
#else
static int kern[6] = {
    -3, 7, 28, 28, 7, -3
};
static int normalizeFactor = 12;

//==========================================================================================
int vStep(UInt8 *inRGB[6], int pos)
{
     return kern[0]*inRGB[0][pos] +
                kern[1]*inRGB[1][pos] +
                kern[2]*inRGB[2][pos] +
                kern[3]*inRGB[3][pos] +
                kern[4]*inRGB[4][pos] +
                kern[5]*inRGB[5][pos];
}

//==========================================================================================
void scale05Lanc6HV(UInt8 *in[6], UInt8 *out,unsigned int width)
{
    int  i, col, refPos;
    int     vstep[6]; //temp array for Vertical Step
    int    outputVal = 0;
    for(i = 0; i < width; i++)
    {
        //Chromas: 1/2 downsampling, so:
        refPos = 2*i;//horizontal position
        //Vertical step:
        for(col = -2; col <= 3; col++)
        {
            vstep[col+2] = vStep(in, refPos+col);
        }
        //Horiz step:
        outputVal = (kern[0] * vstep[0] +
                     kern[1] * vstep[1] +
                     kern[2] * vstep[2] +
                     kern[3] * vstep[3] +
                     kern[4] * vstep[4] +
                     kern[5] * vstep[5]);
        if (outputVal < 0)
        {
            outputVal = 0;
        }
        if (outputVal > 1044480 ) //0xFF000
        {
            outputVal = 1044480;
        }
        outputVal = outputVal >> normalizeFactor;
        out[i] = (UInt8)outputVal;
    }
}
#endif

//##########################################################################################
void svuScl05Lanc6(SippFilter *fptr)
{
    UInt32  plane;

    UInt8 *in[6];
    UInt8 *out;
    UInt32 numPlanes;
    numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Loop through all planes
        for(plane=0; plane < numPlanes; plane++)
        {
            out = (UInt8 *)getPlaneIoPtrs(fptr, 0, iterNum, plane, in);

    #ifdef SIPP_USE_MVCV
            mvispScale05Lanc6HV_asm(in, out, fptr->sliceWidth);
    #else
            scale05Lanc6HV(in, out, fptr->sliceWidth);
    #endif
        }
    }
}
