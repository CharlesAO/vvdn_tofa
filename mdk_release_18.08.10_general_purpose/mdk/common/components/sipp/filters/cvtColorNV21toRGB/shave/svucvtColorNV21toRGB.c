#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorNV21toRGB/cvtColorNV21toRGB.h>


/// cvtColorKernel to conversion NV21 to RGB
/// Performs color space conversion: NV21 to RGB
///@param[in]  yin input Y channel
///@param[in]  uvin input UV channel interleaved
///@param[out] outR output R channel
///@param[out] outG output G channel
///@param[out] outB output B channel
///@param[in]  width - image width in pixels

#ifdef SIPP_USE_MVCV
void mvcvCvtColorNV21toRGB_asm      (UInt8** yin, UInt8** uvin, UInt8** outR, UInt8** outG, UInt8** outB, UInt32 width);
#else
void cvtColorNV21toRGBImplementation(UInt8** yin, UInt8** uvin, UInt8** outR, UInt8** outG, UInt8** outB, UInt32 width)
{
    UInt32 i;

    UInt8* y  = yin[0];
    UInt8* uv = uvin[0];
    UInt8* rr = outR[0];
    UInt8* gg = outG[0];
    UInt8* bb = outB[0];

    UInt32 uvidx = 0;
    int yy,u,v,r,g,b;
    for(i = 0; i < width; i+=2 )
    {
        yy = y[i];
        u = uv[uvidx] - 128;
        v = uv[uvidx+1] - 128;
        uvidx += 2;
            r =  yy + (int)(1.402f*v);
            rr[i] = (UInt8) (r > 255 ? 255 : r < 0 ? 0 : r);
            g =  yy - (int)(0.344f*u + 0.714*v);
            gg[i] = (UInt8) (g > 255 ? 255 : g < 0 ? 0 : g);
            b =  yy + (int)(1.772f*u);
            bb[i] = (UInt8) (b > 255 ? 255 : b < 0 ? 0 : b);
    //----------------------------------------------
        yy = y[i + 1];
        r =  yy + (int)(1.402f*v);
        rr[i + 1] = (UInt8) (r > 255 ? 255 : r < 0 ? 0 : r);
        g =  yy - (int)(0.344f*u + 0.7148*v);
        gg[i + 1] = (UInt8) (g > 255 ? 255 : g < 0 ? 0 : g);
        b =  yy + (int)(1.772f*u);
        bb[i + 1] = (UInt8) (b > 255 ? 255 : b < 0 ? 0 : b);
    }
}
#endif

void svucvtColorNV21toRGB(SippFilter *fptr)
{
    UInt8 *outputR;
    UInt8 *outputG;
    UInt8 *outputB;

    UInt8 *ilineY;
    UInt8 *ilineUV;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 2 input lines
        ilineY = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        ilineUV = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);

        //the output line
        outputR = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outputG = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        outputB = (UInt8 *)getOutPtr(fptr, iterNum, 2);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorNV21toRGB_asm(&ilineY, &ilineUV, &outputR, &outputG, &outputB, fptr->sliceWidth);
    #else
        cvtColorNV21toRGBImplementation(&ilineY, &ilineUV, &outputR, &outputG, &outputB, fptr->sliceWidth);
    #endif
    }
}
