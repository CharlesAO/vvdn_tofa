#include <sipp.h>
#include <sippShaveMacros.h>

float kern[4] = { -1.0f/16.0f,
                   9.0f/16.0f,
                   9.0f/16.0f,
                  -1.0f/16.0f };

//=============================================================================
void upscale2xH(SippFilter *fptr, UInt8 *out)
{
    UInt32 x, p;
    UInt32 planeStride = sippFilterGetPlaneStride(fptr, 0);
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    for(p=0; p<numPlanes; p++)
    {
        for(x=1; x<fptr->outputW; x+=2)
        {
           out[x] = clampU8(kern[0] * out[x-1-2] +
                            kern[1] * out[x-1  ] +
                            kern[2] * out[x+1  ] +
                            kern[3] * out[x+1+2]);
        }

        //Moving to next output chroma plane (R/G/B)
        out += planeStride;
    }
}

//=============================================================================
// V-step computes missing pixels but puts them on the right position in
// output line (i.e. on EVEN positions in output image)
// V-step does a bit of SW padding as required by H-step
//
#define MARGIN 4

void upscale2xV(SippFilter *fptr, UInt8 *in[4], UInt8 *out, int runNo)
{
    int x;
    UInt32 p, ln;
    UInt32 parentPlaneStride = sippFilterGetParentPlaneStride(fptr, 0);
    UInt32 planeStride = sippFilterGetPlaneStride(fptr, 0);
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);

    for(p=0; p<numPlanes; p++)
    {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //Compute current plane
        if((runNo & 1) == 0) //EVEN line
        {//EVEN pixels on EVEN lines (in the output img)
            //have H and V-phase = 0, therefore just copy
            for(x=0-MARGIN;  x<(int)fptr->sliceWidth+MARGIN; x+=2)
                out[x] = in[1][x/2];
        }
        else{ //Odd line: interp
            for(x=0-MARGIN; x<(int)fptr->sliceWidth+MARGIN; x+=2)
            {
                out[x] = clampU8(kern[0] * in[0][x/2] +
                                 kern[1] * in[1][x/2] +
                                 kern[2] * in[2][x/2] +
                                 kern[3] * in[3][x/2] );
            }
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //Move to next plane (R to G, or G to B)
        for(ln=0; ln<4; ln++)
            in[ln] += parentPlaneStride;

        //Moving to next output chroma plane (R/G/B)
        out    += planeStride;
    }
}

//#############################################################################
void svuScl2xLancHV(SippFilter *fptr)
{
    int ln;

    //Input RGB lines (parent[0] must be the 3xplane RGB)
    UInt8 *in[4];
    UInt8 *out;

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
        upscale2xV(fptr, in, out, runNo);
        upscale2xH(fptr, out);
    }
}
