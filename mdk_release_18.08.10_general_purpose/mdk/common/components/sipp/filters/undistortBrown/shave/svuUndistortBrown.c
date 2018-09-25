#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/undistortBrown/undistortBrown.h>

extern UInt32 getInPtrAbs(SippFilter *fptr, UInt32 parNo, UInt32 lineNo, UInt32 planeNo, UInt32 targetSlice);

//#############################################################################
void svuUndistortBrown(SippFilter *fptr)
{
    UInt32             x;
    UndistortBParam   *param = (UndistortBParam *)fptr->params;
    UInt32             svuNo = scGetShaveNumber();
    UInt32             lpi   = sippFilterGetLinesPerIter (fptr);
    UInt32             runNo = fptr->exeNo * lpi;

    //I/O pointers
    UInt8 *in;
    UInt8  interp  = 0;
    UInt32    inLines = sippFilterGetParentInputLines(fptr, 0);
    float  min, r, radialTerm;
    float  xf,    yf;
    float  xCorr, yCorr;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        UInt8 *out     = (UInt8 *)getOutPtr(fptr, iterNum, 0);//WRESOLVE(fptr->dbLineOut[runNo&1], svuNo);

        //Compute min
        if(param->cx < param->cy)
            min = param->cx;
        else
            min = param->cy;

        for(x=0; x<fptr->sliceWidth; x++)
        {
            //Figure out the absolute X coordinates (the Y is absolute already)
            float absX = (svuNo - fptr->gi->sliceFirst)*fptr->sliceWidth + x;
            float absY = runNo;

            //Center coordinates:
            absX -= param->cx;
            absY -= param->cy;

            xf = absX / min;
            yf = absY / min;

            r = sqrtf(xf*xf + yf*yf);

            radialTerm = 1.0f + param->k1 * r * r
                              + param->k2 * r * r * r * r;

            xCorr = xf * radialTerm + param->p1 * (r*r + 2*xf*xf) + 2*param->p2 * xf * yf;
            yCorr = yf * radialTerm + param->p2 * (r*r + 2*yf*yf) + 2*param->p1 * xf * yf;

            xCorr *= min;
            yCorr *= min;

            //Back to abs coords
            xCorr += param->cx;
            yCorr += param->cy;

            //Interpolate/sample the data at UV coords
            if ( (xCorr<0)||(xCorr>fptr->outputW) ||
                 (yCorr<0)||(yCorr>fptr->outputH) )
            {
              //if U,V are out of image coord space, put some default color
                interp = 0;
            }
            else
            {   //Pixel coords are within valid range, interpolate
                //for now, just use a dummy nearest neighbor implementation

                //Here's the trick: we don't assume data is in current slice,
                //so need to compute the absolute address of the pixels of interest
                //which depends on the slices

                //Important, V is an absolute value, but find the v relative to
                int slice  = ((int)xCorr) / fptr->sliceWidth + fptr->gi->sliceFirst;
                int offset = ((int)xCorr) % fptr->sliceWidth; //offset within the slice

                //in = (UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][inLines/2 + ((int)yCorr - runNo)], slice);
                in = (UInt8*)getInPtrAbs(fptr, 0, iterNum, inLines/2 + ((int)yCorr - runNo), 0, slice);

                //Interpolation: nearest neighbor, could do more complex
                //normally would need to consider fractional parts of u, v here
                //and do bilinear or bicubic interpolation
                interp = in[offset];
            }

            out[x] = interp;
        }
    }
}