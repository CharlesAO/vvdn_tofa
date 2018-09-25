#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genLuma/genLuma.h>

//##########################################################################################
void genLuma(UInt8* inR, UInt8 *inG, UInt8 *inB, UInt8 *outY, int width)
{
    int i;
    for(i=0; i<width; i++)
    {
        outY[i] = (inR[i]*0.25f + inG[i]*0.50f + inB[i]*0.25f);
    }
}

//##########################################################################################
void svuGenLuma(SippFilter *fptr)
{
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Input RGB lines
        UInt8 *inR = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 0);
        UInt8 *inG = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 1);
        UInt8 *inB = (UInt8*)getInPtr(fptr, 0, iterNum, 0, 2);

        //Output U8f
        UInt8 *outY = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #if defined(SIPP_PC)
        genLuma(inR, inG, inB, outY, fptr->sliceWidth);
    #elif defined(__myriad2__)//moviCompile
        extern void genLumaOpt(UInt8* inR, UInt8 *inG, UInt8 *inB, UInt8 *outY, int width);
        genLumaOpt(inR, inG, inB, outY, fptr->sliceWidth);
    #endif
    }
}

