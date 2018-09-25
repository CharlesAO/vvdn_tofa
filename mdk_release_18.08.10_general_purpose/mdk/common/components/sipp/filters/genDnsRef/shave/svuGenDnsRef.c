#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genDnsRef/genDnsRef.h>

#if defined(DNS_REF_USE_LOCAL_DATA)
UInt8 slcLocalDistLut [256] ALIGNED(8);
UInt8 slcLocalGammaLut[256] ALIGNED(8);
#endif

//##########################################################################################
void genDnsRef(UInt8 *inY, UInt32 yc, UInt32 x0,  YDnsRefParam* param, UInt8* outRef, UInt32 width)
{
    UInt32 x, xc;

    for(x=0; x<width; x++)
    {
        xc = (x0 + x);
        xc = xc * xc;
        outRef[x] = ((UInt32)param->lutGamma[inY[x]] * param->lutDist[(xc+yc)>>param->shift])>>8;
    }
}

//##########################################################################################
void svuGenDnsRef(SippFilter *fptr)
{
    int    x0;
    int    yc; //centered Squared Y coord (center relative)

    YDnsRefParam *param = (YDnsRefParam *)fptr->params;
    UInt32       svuNo  = scGetShaveNumber();
    UInt32       lpi    = sippFilterGetLinesPerIter (fptr);
    UInt32       runNo  = fptr->exeNo * lpi;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        UInt8        *inY   = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0); //Input Y line
        UInt8        *outY  = (UInt8 *)getOutPtr(fptr, iterNum, 0);      //Output Y ref

    #if defined(DNS_REF_USE_LOCAL_DATA)
        //Note: application needs to initialize these luts before use !
        param->lutGamma = slcLocalGammaLut;
        param->lutDist  = slcLocalDistLut;
    #endif

        //Y coord is constant for entire line, so can compute here.
        yc = (runNo - fptr->outputH/2);
        yc = yc * yc;
        //x0 = constant
        x0 = ((svuNo-fptr->gi->sliceFirst) * fptr->sliceWidth) - fptr->outputW/2;

    #if defined(__myriad2__)
        extern void genDnsRefOpt(UInt8 *inY, UInt32 yc, UInt32 x0,  UInt8* param, UInt8* outRef, UInt32 width);
        genDnsRefOpt(inY, yc, x0, (UInt8*)param, outY, fptr->sliceWidth);
    #else
        genDnsRef(inY, yc, x0, param, outY, fptr->sliceWidth);
    #endif
    }
}
