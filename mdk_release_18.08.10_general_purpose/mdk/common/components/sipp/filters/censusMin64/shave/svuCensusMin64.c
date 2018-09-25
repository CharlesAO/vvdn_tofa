#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMin64/censusMin64.h>

/// mvcvCensusMin - computes minimum of 64 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of minimum positions
/// @param[out] outm          - array of minimum values
/// @param[in] width          - width of the input lines
/// @return    Nothing
#ifdef SIPP_USE_MVCV
void mvcvCensusMin64_asm(UInt8 *in, UInt8 *out, UInt8 *outm, UInt32 width);
#else
void mvcvCensusMin64(UInt8 *in, UInt8 *out, UInt8 *outm, UInt32 width)
{
    UInt32 j, disp = 64;
    UInt8 temp[7][16];
    UInt8 pos [7][16];
    for (UInt32 i = 0; i < width; i++)
    { // for all pixels


        for (j=0;j<16;j++)
        {
            temp[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  in[i*disp +j] : in[i*disp +j + 16];
             pos[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  j : j+16;

            temp[1][j] = (in[i*disp +j + 32] <= in[i*disp +j + 48]) ?  in[i*disp +j + 32] : in[i*disp +j + 48];
             pos[1][j] = (in[i*disp +j + 32] <= in[i*disp +j + 48]) ?  j + 32 : j + 48;
        }

        for (j=0;j<16;j++)
        {
            temp[2][j] = (temp[0][j] <= temp[1][j] ) ? temp[0][j] : temp[1][j];
            pos[2][j] =  (temp[0][j] <= temp[1][j] ) ? pos[0][j] : pos[1][j];
        }

        for (j=0;j<8;j++)
        {
            temp[3][j] = (temp[2][j] <= temp[2][j+8] ) ? temp[2][j] : temp[2][j+8];
             pos[3][j] = (temp[2][j] <= temp[2][j+8] ) ? pos[2][j] : pos[2][j+8];
        }

        for (j=0;j<4;j++)
        {
            temp[4][j] = (temp[3][j] <= temp[3][j+4] ) ? temp[3][j] : temp[3][j+4];
             pos[4][j] = (temp[3][j] <= temp[3][j+4] ) ? pos[3][j] : pos[3][j+4];
        }

        for (j=0;j<2;j++)
        {
            temp[5][j] = (temp[4][j] <= temp[4][j+2] ) ? temp[4][j] : temp[4][j+2];
             pos[5][j] = (temp[4][j] <= temp[4][j+2] ) ? pos[4][j] : pos[4][j+2];
        }

            temp[6][0] = (temp[5][1] < temp[5][0] ) ? temp[5][1] : temp[5][0];
             pos[6][0] = (temp[5][1] < temp[5][0] ) ? pos[5][1] : pos[5][0];


        // Store the position of min
        out[i] = pos[6][0];
        outm[i] = temp[6][0];
    }
}
#endif

void svuCensusMin64(SippFilter *fptr)
{
    UInt8 *input;
    UInt8 *output;
    UInt8 *outputM;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output  = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        //get second output
        u32 curSvu = scGetShaveNumber();
        u32 runNo  = fptr->exeNo&1;
        s32 addr   = (s32)fptr->dbLineOut[1][runNo][iterNum];
        addr      += fptr->outOff[1][curSvu * fptr->nPlanes[1]];
        outputM    = (UInt8 *)addr;

    #ifdef SIPP_USE_MVCV
        mvcvCensusMin64_asm(input, output, outputM, fptr->sliceWidth);
    #else
        mvcvCensusMin64(input, output, outputM, fptr->sliceWidth);
    #endif

    }
}
