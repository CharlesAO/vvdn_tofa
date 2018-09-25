#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/boxFilter5x5/boxFilter5x5.h>

/// boxfilter kernel that makes average on 5x5 kernel size
/// @param[in] in        - array of pointers to input lines
/// @param[out] out      - array of pointers for output lines
/// @param[in] normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
/// @param[in] width     - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBoxfilter5x5_asm(UInt8** in, UInt8** out, UInt32 normalize, UInt32 width);
#else
void boxfilter5x5        (UInt8** in, UInt8** out, UInt32 normalize, UInt32 width)
{
    int i,x,y;
    UInt8* lines[5];
    unsigned int sum;
    unsigned short* aux;
    aux=(unsigned short *)(*out);

    //Initialize lines pointers
    lines[0] = *in;
    lines[1] = *(in+1);
    lines[2] = *(in+2);
    lines[3] = *(in+3);
    lines[4] = *(in+4);

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 5; y++)
        {
            for (x = -2; x <= 2; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(UInt8)(((half)(float)sum)*(half)0.04);
        }
        else if (normalize == 2)  //andreil: test mode
        {
            *(*out+i)=(UInt8)(sum/25.0f);
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
}
#endif

void svuBoxFilter5x5(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[5];
    UInt32 plane;
    BoxFilter5x5Param *param = (BoxFilter5x5Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane=0; plane<numPlanes; plane++)
        {
            //the output line
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

        #ifdef SIPP_USE_MVCV
            mvcvBoxfilter5x5_asm(iline, &output, param->normalize, fptr->sliceWidth);
        #else
            boxfilter5x5(iline, &output, param->normalize, fptr->sliceWidth);
        #endif
        }
    }
}
