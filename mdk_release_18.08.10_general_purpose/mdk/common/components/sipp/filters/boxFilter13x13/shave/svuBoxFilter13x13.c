#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/boxFilter13x13/boxFilter13x13.h>

/// boxfilter kernel that makes average on 13x13 kernel size
/// @param[in] in        - array of pointers to input lines
/// @param[out] out      - array of pointers for output lines
/// @param[in] normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
/// @param[in] width     - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBoxfilter13x13_asm(UInt8** in, UInt8** out, UInt32 normalize, UInt32 width);
#else
void boxfilter13x13        (UInt8** in, UInt8** out, UInt32 normalize, UInt32 width)
{
    int i,x,y;
    UInt8* lines[13];
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
    lines[11] = *(in+11);
    lines[12] = *(in+12);

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (y = 0; y < 13; y++)
        {
            for (x = -6; x <= 6; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(UInt8)(((half)(float)sum)*(half)(0.00591715976331360946745562130178));
        }
        else if (normalize == 2) //andreil: test mode
        {
            *(*out+i)=(UInt8)(sum/169.0f);
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
}
#endif

void svuBoxFilter13x13(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[13];
    UInt32 plane;
    UInt32 numPlanes;
    numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    BoxFilter13x13Param *param = (BoxFilter13x13Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane < numPlanes; plane++)
        {
        //the output line
        output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
        mvcvBoxfilter13x13_asm(iline, &output, param->normalize, fptr->sliceWidth);
    #else
        boxfilter13x13(iline, &output, param->normalize, fptr->sliceWidth);
    #endif
        }
    }
}
