#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/combDecimDemosaicAwbGains/combDecimDemosaicAwbGains.h>

#define CLAMPZ255(X) ((X)>255 ? 255 : ((X)<0 ? 0 : (X)))

void combDecimDemosaicAwbGainsGR(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
{
    int i;
    UInt16 pixR;
    UInt16 pixG;
    UInt16 pixB;
    iline[1][-1] = iline[1][1];
    iline[3][-1] = iline[3][1];
    for(i=0; i<(int)width; i++)
    {
        pixR =   ((iline[1][i*2-1] * 3 +  // R
                 iline[1][i*2+1] * 9 +
                 iline[3][i*2-1] * 1 +
                 iline[3][i*2+1] * 3) >> 4 );

        pixG =   ((iline[1][i*2  ] +  // G
                 iline[2][i*2+1] + 1) >> 1);

        pixB =   ((iline[0][i*2  ] * 3 +  // B
                 iline[0][i*2+2] * 1 +
                 iline[2][i*2  ] * 9 +
                 iline[2][i*2+2] * 3) >> 4);
        // apply gains
        output[0][i] = CLAMPZ255((((unsigned int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
        output[1][i] = CLAMPZ255((((unsigned int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
        output[2][i] = CLAMPZ255((((unsigned int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];
}

void combDecimDemosaicAwbGainsRG(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
{
    int i;
    UInt16 pixR;
    UInt16 pixG;
    UInt16 pixB;
    iline[0][-1] = iline[0][1];
    iline[2][-1] = iline[2][1];
    for(i=0; i<(int)width; i++)
    {
        pixR =   ((iline[1][i*2    ] * 9 +  // R
                 iline[1][i*2 +   2] * 3 +
                 iline[3][i*2      ] * 3 +
                 iline[3][i*2 +   2] * 1) >> 4 );

        pixG =   ((iline[1][i*2 + 1] +  // G
                 iline[2][i*2      ] + 1) >> 1);

        pixB =   ((iline[0][i*2 + 1] * 3 +  // B
                 iline[0][i*2   - 1] * 1 +
                 iline[2][i*2   + 1] * 9 +
                 iline[2][i*2   - 1] * 3) >> 4);
        // apply gains
        output[0][i] = CLAMPZ255((((unsigned int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
        output[1][i] = CLAMPZ255((((unsigned int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
        output[2][i] = CLAMPZ255((((unsigned int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];
}

void combDecimDemosaicAwbGainsGB(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
{
    int i;
    UInt16 pixR;
    UInt16 pixG;
    UInt16 pixB;
    iline[1][-1] = iline[1][1];
    iline[3][-1] = iline[3][1];
    for(i=0; i<(int)width; i++)
    {
        pixR =   ((iline[0][i*2    ] * 3 +  // R
                 iline[0][i*2 +   2] * 1 +
                 iline[2][i*2      ] * 9 +
                 iline[2][i*2 +   2] * 3) >> 4 );

        pixG =   ((iline[1][i*2    ] +  // G
                 iline[2][i*2 +   1] + 1) >> 1);

        pixB =   ((iline[1][i*2 + 1] * 9 +  // B
                 iline[1][i*2   - 1] * 3 +
                 iline[3][i*2   + 1] * 3 +
                 iline[3][i*2   - 1] * 1) >> 4);
        // apply gains
        output[0][i] = CLAMPZ255((((unsigned int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
        output[1][i] = CLAMPZ255((((unsigned int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
        output[2][i] = CLAMPZ255((((unsigned int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];
}

void combDecimDemosaicAwbGainsBG(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
{
    int i;
    UInt16 pixR;
    UInt16 pixG;
    UInt16 pixB;
    iline[0][-1] = iline[0][1];
    iline[2][-1] = iline[2][1];
    for(i=0; i<(int)width; i++)
    {
        pixR =   ((iline[0][i*2 - 1] * 1 +  // R
                 iline[0][i*2 +   1] * 3 +
                 iline[2][i*2 -   1] * 3 +
                 iline[2][i*2 +   1] * 9) >> 4 );

        pixG =   ((iline[1][i*2 + 1] +  // G
                 iline[2][i*2      ] + 1) >> 1);

        pixB =   ((iline[1][i*2    ] * 9 +  // B
                 iline[1][i*2 +   2] * 3 +
                 iline[3][i*2      ] * 3 +
                 iline[3][i*2 +   2] * 1) >> 4);
        // apply gains
        output[0][i] = CLAMPZ255((((unsigned int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
        output[1][i] = CLAMPZ255((((unsigned int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
        output[2][i] = CLAMPZ255((((unsigned int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];
}


void svuCombDecimDemosaicAwbGains(SippFilter *fptr)
{
    UInt8 *output[3];
    UInt16 *iline[4];
    CombDecimAwbGainsParam* params = (CombDecimAwbGainsParam*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi         = sippFilterGetLinesPerIter (fptr);
    UInt32 localExeNo  = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, localExeNo++)
    {

        if(0 == localExeNo)
        {
            iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
            iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
            iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
            iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 3, 0);
        }
        else
        {
            if((fptr->outputH-1) == localExeNo)
            {
                iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
                iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
                iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
                iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
            }
            else
            {
                iline[0]=(UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
                iline[1]=(UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
                iline[2]=(UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);
                iline[3]=(UInt16 *)getInPtr(fptr, 0, iterNum, 3, 0);
            }
        }

        output[0] = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        output[1] = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        output[2] = (UInt8 *)getOutPtr(fptr, iterNum, 2);

        switch(params->bayerOrder)
        {
        case BAYER_FORMAT_GRBG:
            combDecimDemosaicAwbGainsGR(output, iline, params->gains, fptr->sliceWidth);
            break;
        case BAYER_FORMAT_RGGB:
            combDecimDemosaicAwbGainsRG(output, iline, params->gains, fptr->sliceWidth);
            break;
        case BAYER_FORMAT_GBRG:
            combDecimDemosaicAwbGainsGB(output, iline, params->gains, fptr->sliceWidth);
            break;
        case BAYER_FORMAT_BGGR:
            combDecimDemosaicAwbGainsBG(output, iline, params->gains, fptr->sliceWidth);
            break;
        default:
            break;
        }
    }
}
