#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/channelExtract/channelExtract.h>

/// channelExtract kernel - This kernel extracts one of the R, G, B, plane from an interleaved RGB line
/// @param[in] in         - array of pointers to input lines
/// @param[out]out        - array of pointers for output lines
/// @param[in] width      - width of input line
/// @param[in] plane      - number 0 to extract plane R, 1 for extracting G, 2 for extracting B
#ifdef SIPP_USE_MVCV
void mvcvChannelExtract_asm(UInt8** in, UInt8** out, UInt32 width, UInt32 plane);
#else
void channelExtract        (UInt8** in, UInt8** out, UInt32 width, UInt32 plane)
{
    UInt8 *in_line;
    UInt8 *out_line;
    UInt32 i, c=0;
    in_line  = *in;
    out_line = *out;

    for (i=plane;i<width;i=i+3)
    {
        out_line[c]=in_line[i];
        c++;
    }
}
#endif

void svuChannelExtract(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input[1];
    ChannelExtractParam *param = (ChannelExtractParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input[0] =(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvChannelExtract_asm(input, &output, (fptr->sliceWidth)*3, param->plane);
    #else
        channelExtract(input, &output, (fptr->sliceWidth)*3, param->plane);
    #endif
    }
}
