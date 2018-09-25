#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/boxFilter11x11/boxFilter11x11.h>

/// boxfilter kernel that makes average on 11x11 kernel size
/// @param[in] in        - array of pointers to input lines
/// @param[out] out      - array of pointers for output lines
/// @param[in] normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
/// @param[in] width     - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBoxfilter11x11_asm(UInt8** in, UInt8** out, UInt32 normalize, UInt32 width);
#else
void boxfilter11x11        (UInt8** in, UInt8** out, UInt32 normalize, UInt32 width)
{
    int i,x,y;
    UInt8* lines[11];
    unsigned int sum;
    unsigned short* aux;
    aux=(unsigned short *)(*out);

    //Initialize lines pointers
    lines[0] = *in;
    lines[1] = *(in+1);
    lines[2] = *(in+2);
    lines[3] = *(in+3);
    lines[4] = *(in+4);
    lines[5] = *(in+5);
    lines[6] = *(in+6);
    lines[7] = *(in+7);
    lines[8] = *(in+8);
    lines[9] = *(in+9);
    lines[10] = *(in+10);

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 11; y++)
        {
            for (x = -5; x <= 5; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(UInt8)(((half)(float)sum)*(half)0.00826446280991735537190082644628);
        }
        else if (normalize == 2) //andreil: test mode
        {
            *(*out+i)=(UInt8)(sum/121.0f);
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
}
#endif

//#######################################################################
void svuBoxFilter11x11(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[11];
    UInt32 plane;
    UInt32 numPlanes;
    numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    BoxFilter11x11Param *param = (BoxFilter11x11Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane < numPlanes; plane++)
        {
      //the output line
        output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
        mvcvBoxfilter11x11_asm(iline, &output, param->normalize, fptr->sliceWidth);
    #else
        boxfilter11x11(iline, &output, param->normalize, fptr->sliceWidth);
    #endif
        }
    }
}
