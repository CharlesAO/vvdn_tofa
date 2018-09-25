#include <sipp.h>
#include <sippShaveMacros.h>

static float kern[4] = { -1.0f/16.0f,
                          9.0f/16.0f,
                          9.0f/16.0f,
                         -1.0f/16.0f };

//#############################################################################
void svuScl2xLancV(SippFilter *fptr)
{
    UInt32  x, ln, p;

    //Input RGB lines (parent[0] must be the 3xplane RGB)
    UInt8 *in[4];
    UInt8 *out;
    UInt32 numPlanes;
    numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //Here: must WRESOLVE all 4 lines required
        //Initially iline_RGB[] point to RED; will add fptr->parents[0]->planeStride
        //          to move to next input plane
        for(ln=0; ln<4; ln++)
        {
            in[ln] = (UInt8 *)getInPtr(fptr, 0, iterNum, ln, 0);
        }
        //Initially points to CR, will add fptr->planeStride to go to next plane
        out = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        //The low level func implementation
        for(p=0; p < numPlanes; p++)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //Compute current plane
            if((runNo & 1) == 0)
            {//Odd line, just copy paste
                for(x=0; x<fptr->sliceWidth; x++)
                    out[x] = in[1][x];
            }
            else
            {
                for(x=0; x<fptr->sliceWidth; x++)
                {
                    out[x] = clampU8(kern[0] * in[0][x] +
                                     kern[1] * in[1][x] +
                                     kern[2] * in[2][x] +
                                     kern[3] * in[3][x] );
                }
            }

            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //Move to next plane (R to G, or G to B)
            for(ln=0; ln<4; ln++)
                in[ln] += fptr->parInfo[0].planeStride;

            //Moving to next output chroma plane (R/G/B)
                out += fptr->planeStride;
       }
    }
}
