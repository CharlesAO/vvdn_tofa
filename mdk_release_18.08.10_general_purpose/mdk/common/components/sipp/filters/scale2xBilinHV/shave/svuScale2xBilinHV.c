#include <sipp.h>
#include <sippShaveMacros.h>

//#############################################################################
void svuScl2xBilinHV(SippFilter *fptr)
{
    UInt32  p, x;
    //Input RGB lines (parent[0] must be the 3xplane RGB)
    UInt8 *in[2];
    UInt8 *out;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr,0);
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++,runNo++)
    {
        //The low level implementation
        for(p=0; p<numPlanes; p++)
        {
            out = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, p/*plane*/, in);
            //================================
            //1) compute current plane
            if((runNo & 1) == 0)
            {//Even line (Y phase = 0, thus only need to interpolate on Horizontal
                for(x=0; x<fptr->sliceWidth; x+=2)
                {
                    out[x+0] = ((int)in[0][x/2]);
                    out[x+1] = ((int)in[0][x/2] + (int)in[0][x/2+1] + 1)>>1;
                }
            }
            else
            {//Odd line, Y phase = 0.5 always
                for(x=0; x<fptr->sliceWidth; x+=2)
                {
                    out[x+0] = ((int)in[0][x/2] + (int)in[1][x/2] + 1)>>1;

                    out[x+1] = ((int)in[0][x/2] + (int)in[0][x/2+1] +
                                (int)in[1][x/2] + (int)in[1][x/2+1] + 3) >> 2;
                }
            }

          //================================
          //2) Move to next plane
       }
    }
}
