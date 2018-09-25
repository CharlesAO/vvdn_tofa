#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/contrast/contrast.h>

//##########################################################################################
void svuContrast(SippFilter *fptr)
{
    UInt32 i;
    ContrastParam *param = (ContrastParam *)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //Input RGB lines
        UInt8 *inR  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        UInt8 *inG  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        UInt8 *inB  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //Output RGB lines
        UInt8 *outR = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        UInt8 *outG = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        UInt8 *outB = (UInt8 *)getOutPtr(fptr, iterNum, 2);

        //Contrast
        for(i=0; i<fptr->sliceWidth; i++)
        {
    #if 1
            //img = double(img - idxLow) * scale;
            outR[i] = clampU8((inR[i] - param->idxLow) * param->scale);
            outG[i] = clampU8((inG[i] - param->idxLow) * param->scale);
            outB[i] = clampU8((inB[i] - param->idxLow) * param->scale);
    #else  //copy-paste for test...
            outR[i] = inR[i];
            outG[i] = inG[i];
            outB[i] = inB[i];
    #endif
        }
    }
}