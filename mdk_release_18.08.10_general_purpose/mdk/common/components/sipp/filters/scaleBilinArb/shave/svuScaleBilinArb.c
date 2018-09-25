#include <sipp.h>
#include <sippShaveMacros.h>

//#############################################################################
void svuSclBilinArb(SippFilter *fptr)
{
    UInt32  p, x;

    UInt32 parOutputW = sippFilterGetParentOutputWidth (fptr, 0);
    UInt32 parOutputH = sippFilterGetParentOutputHeight (fptr, 0);

    float   ratioH = (float)parOutputW / fptr->outputW;
    float   ratioV = (float)parOutputH / fptr->outputH;

    //I/O line pointers
    UInt8 *in[2];
    UInt8 *out;

    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);

    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //The two input lines needed (bilinear)
        in[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        in[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);

        //Initially, point to first plane, will add fptr->planeStride to go to next plane
        out   = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        //Compute in advance yFrac, as it's constant for entire line
        float dyFrac = runNo * ratioV;
        dyFrac       = dyFrac - (int)dyFrac;


        //================================================================================================
        //The low level implementation
        for(p=0; p<numPlanes; p++)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //1) compute current plane
            for(x=0; x<fptr->sliceWidth; x++)
            {
                float xCoord = (float)x * ratioH;
                int   xWhole = (int)xCoord;
                float xFrac  = xCoord-(float)xWhole;

                //The weights
                float BW[4] = { (1-xFrac)*(1-dyFrac),
                                (  xFrac)*(1-dyFrac),
                                (1-xFrac)*(  dyFrac),
                                (  xFrac)*(  dyFrac)};

                //Bilinear interp
                out[x+0] = in[0][xWhole] * BW[0] + in[0][xWhole+1] * BW[1]+ //line 0
                           in[1][xWhole] * BW[2] + in[1][xWhole+1] * BW[3]; //line 1
            }

            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
           //2) Move to next plane
            UInt32 parentPlaneStride = sippFilterGetParentPlaneStride(fptr, 0);
            UInt32 planeStride = sippFilterGetPlaneStride(fptr, 0);
            in[0] += parentPlaneStride;
            in[1] += parentPlaneStride;
            out   += planeStride;
        }
    }
}
