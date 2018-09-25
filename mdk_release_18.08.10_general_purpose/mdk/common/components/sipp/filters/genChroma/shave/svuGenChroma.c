#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genChroma/genChroma.h>

//#define CHROMA_EPSILON (1.0f/1024.0f)

//Octave ref
//  oo_luma = 1 ./ (presharp + cfg.chroma_epsilon);
//  for i = 1:3
//      ci(:,:,i) = lanczos_downsample(img(:,:,i) .* oo_luma * (255/3));
//  end

//##########################################################################################
void genChroma(UInt8* inRGB[3], UInt8 *inY, UInt8* outC[3], int width, int eps)
{
    UInt8 *inR   = inRGB[0];
    UInt8 *inG   = inRGB[1];
    UInt8 *inB   = inRGB[2];

    UInt8  *outCR = outC[0];
    UInt8  *outCG = outC[1];
    UInt8  *outCB = outC[2];

    int i;
    UInt32 ooLuma;

    for(i=0; i<width; i++)
    {   //div oo_luma by extra amount, to scale from input #bits to 8bit output
        ooLuma   =  (inY[i] + eps);
        outCR[i] =  clampU8(inR[i] * (255/3) / ooLuma);
        outCG[i] =  clampU8(inG[i] * (255/3) / ooLuma);
        outCB[i] =  clampU8(inB[i] * (255/3) / ooLuma);
    }
}

//##########################################################################################
void svuGenChroma(SippFilter *fptr)
{
    UInt8 *inRGB[3]; //Input RGB lines (parent[0] must be the 3xplane RGB)
    UInt8 *inY ;     //Input second parent
    UInt8  *outC[3]; //Output Chroma
    int    eps   = ((ChrGenParam*)fptr->params)->epsilon;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        inRGB[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inRGB[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inRGB[2] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        inY      = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        outC[0]  = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outC[1]  = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        outC[2]  = (UInt8 *)getOutPtr(fptr, iterNum, 2);

    #if defined(__myriad2__)//movicompile M2
        extern void genChromaOpt(UInt8* inRGB[3], UInt8 *inY, UInt8* outC[3], int width, int eps);
        genChromaOpt(inRGB, inY, outC, fptr->sliceWidth, eps);
    #else
        genChroma(inRGB, inY, outC, fptr->sliceWidth, eps);
    #endif
    }
}
