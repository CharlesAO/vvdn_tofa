#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/boxFilter/boxFilter.h>

//#####################################################################################################
//General template...
#define BOX_FILT(ACC_TYPE, DATA_TYPE)                          \
    for(curPix=0; curPix<(int)width; curPix++)                      \
    {                                                          \
        ACC_TYPE sum = 0;                                      \
        for(y=0; y<(int)kerSzV; y++)                                \
            for(x=-lobeH; x<= (int)lobeH; x++)                      \
                sum += *( ((DATA_TYPE*)in[y]) + x + curPix);   \
        if(normalize)                                          \
            ((DATA_TYPE*)out)[curPix] = sum/norm;              \
        else                                                   \
            ((DATA_TYPE*)out)[curPix] = sum;                   \
    }
//#####################################################################################################
void boxfilter(void* in[], void* out, UInt32 dataFmt, UInt32 kerSzH, UInt32 kerSzV, UInt32 normalize, UInt32 width)
{
    int lobeH = kerSzH>>1;
    int norm  = kerSzH * kerSzV;
    int curPix,x,y;

    switch(dataFmt)
    {
        case FMT_U8 : BOX_FILT(int,   UInt8 ); break;
        case FMT_U16: BOX_FILT(int,   UInt16); break;
        case FMT_U32: BOX_FILT(int,   UInt32); break;
        case FMT_F16: BOX_FILT(float, half);   break;
        case FMT_F32: BOX_FILT(float, float);  break;
    }
}

//#####################################################################################################
void svuBoxFilter(SippFilter *fptr)
{
    void *output;
    void *iLine[BOX_MAX_V_SZ];
    BoxFilterParam *param = (BoxFilterParam*)fptr->params;
    UInt32 p;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

    #if defined(SIPP_PC)
        //we allocate line pointers on stack, so make sure height < what gets allocate
        sippAssert(param->filterSizeV < BOX_MAX_V_SZ, 555);
    #endif

        //Operate on multiple planes, one at a time:
        for(p=0; p<numPlanes; p++)
        {
    #if 0
            //Get input pointers
            for(y=0; y<param->filterSizeV; y++)
            {
                iLine[y] = (void *)getInPtr(fptr, 0, y, p);
            }
            //The output line
            output = (void *)getOutPtr(fptr, p);
    #else
            //Get Input and output line in an optimal way
            output = (void*)getPlaneIoPtrs(fptr, 0, iterNum, p, iLine);
    #endif

            //The implementation
            boxfilter(iLine, output, param->dataFormat,
                                     param->filterSizeH,
                                     param->filterSizeV,
                                     param->normalize,
                                     fptr->sliceWidth);

            //Based on all these params, custom ASM implementations could be called if exist ...
        }
    }
}
