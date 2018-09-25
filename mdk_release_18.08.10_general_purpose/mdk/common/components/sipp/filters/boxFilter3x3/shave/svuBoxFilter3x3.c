#include <sipp.h>
#include <sippInternal.h>
#include <sippShaveMacros.h>
#include <filters/boxFilter3x3/boxFilter3x3.h>



/// boxfilter kernel that makes average on 3x3 kernel size
/// @param[in] in        - array of pointers to input lines
/// @param[out] out      - array of pointers for output lines
/// @param[in] normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
/// @param[in] width     - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBoxfilter3x3_asm(UInt8** in, UInt8** out, UInt32 normalize, UInt32 width);
#else
void boxfilter3x3        (UInt8** in, UInt8** out, UInt32 normalize, UInt32 width)
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

    //Go on the whole line
    for (i = 0; i < (int)width; i++)
    {
        sum = 0;
        for (y = 0; y < 3; y++)
        {
            for (x = -1; x <= 1; x++)
            {
                sum += (lines[y][x]);
            }
            lines[y]++;
        }

        if(normalize == 1)
        {
            *(*out+i)=(UInt8)(((half)(float)sum)*(half)0.1111);
        }
        else if (normalize == 2)
        {
            *(*out+i) = sum / 9; //test mode (andreil)
        }
        else
        {
            *(aux+i)=(unsigned short)sum;
        }
    }
}
#endif

void svuBoxFilter3x3(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[3];
    UInt32 plane;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    BoxFilter3x3Param *param = (BoxFilter3x3Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane=0; plane < numPlanes; plane++)
        {
            //the output line
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvBoxfilter3x3_asm(iline, &output, param->normalize, fptr->sliceWidth);
    #else
            boxfilter3x3(iline, &output, param->normalize, fptr->sliceWidth);
    #endif
        }
    }
}
