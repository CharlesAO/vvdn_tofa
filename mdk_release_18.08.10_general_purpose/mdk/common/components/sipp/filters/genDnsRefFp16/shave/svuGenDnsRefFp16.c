#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genDnsRefFp16/genDnsRefFp16.h>

#if defined(DNS_REF_USE_LOCAL_DATA)
UInt8 slcLocalDistLut [256] ALIGNED(8);
UInt8 slcLocalGammaLut[256] ALIGNED(8);
#endif

//##########################################################################################
#if defined(SIPP_USE_MVCV)
void mvispGenDnsRefFp16_asm(half *inY, unsigned int yc, int x0,  YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width);
#else
void genDnsRefFp16         (half *inY, unsigned int yc, int x0,  YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width)
{
    int x, xc;
    unsigned char idxLuma;
    for(x=0; x<(int)width; x++)
    {
        xc = (x0 + x);
        xc = xc * xc;
        // convert luma to u8 in order to be use as index in lut
        idxLuma = (unsigned char)(inY[x] * 255.0f);
        unsigned short value = (unsigned short)param->lutGamma[idxLuma] *
            (unsigned short)param->lutDist[(((xc+yc)>>param->shift))];
        value = value >> 8;
        outRef[x] = (unsigned char)value;
    }
}
#endif

//##########################################################################################
void svuGenDnsRefFp16(SippFilter *fptr)
{
    int    x0;
    int    yc; //centered Squared Y coord (center relative)
    YDnsRefFp16Param *param = (YDnsRefFp16Param *)fptr->params;
    UInt32       svuNo  = scGetShaveNumber();
    UInt32       lpi    = sippFilterGetLinesPerIter (fptr);
    UInt32       runNo  = fptr->exeNo * lpi;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        half         *inY   = (half *)getInPtr(fptr, 0, iterNum, 0, 0); //Input Y line
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
    #if 1 //Single pipeline mode requires this:
        x0 = ((svuNo-fptr->gi->sliceFirst) * fptr->sliceWidth) - fptr->outputW/2;
    #else //Split scheme requires this:
        x0 = ((svuNo-fptr->gi->sliceFirst) * fptr->sliceWidth) + param->xOffset;
    #endif


    #if defined(SIPP_USE_MVCV)
        mvispGenDnsRefFp16_asm(inY, yc, x0, param, outY,  fptr->sliceWidth);
    #else
        genDnsRefFp16         (inY, yc, x0, param, outY,  fptr->sliceWidth);
    #endif
    }
}
