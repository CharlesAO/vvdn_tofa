#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convYuv444/convYuv444.h>
//this scale factor is used to up half[0..1] to UInt8
#define SCALE 255.0f

//##########################################################################################
void svuRgbYuv444(SippFilter *fptr)
{
    UInt32 i;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //Input RGB lines
        half *inR = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        half *inG = (half *)getInPtr(fptr, 0, iterNum, 0, 1);
        half *inB = (half *)getInPtr(fptr, 0, iterNum, 0, 2);

        //Output lines
        UInt8 *outY  = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        UInt8 *outU  = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        UInt8 *outV  = (UInt8 *)getOutPtr(fptr, iterNum, 2);

        //Conversion:
        for(i=0; i<fptr->sliceWidth; i++)
        {
            outY[i] = clampZ255(inR[i] * ( 0.299f*SCALE) + inG[i]* ( 0.587f*SCALE) + inB[i] * ( 0.114f *SCALE)         );
            outU[i] = clampZ255(inR[i] * (-0.169f*SCALE) + inG[i]* (-0.332f*SCALE) + inB[i] * ( 0.500f *SCALE) + 128.0f);
            outV[i] = clampZ255(inR[i] * ( 0.500f*SCALE) + inG[i]* (-0.419f*SCALE) + inB[i] * (-0.0813f*SCALE) + 128.0f);
        }
    }
}
