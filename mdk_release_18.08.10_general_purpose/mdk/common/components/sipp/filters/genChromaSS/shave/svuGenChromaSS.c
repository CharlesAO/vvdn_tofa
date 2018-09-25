#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/genChromaSS/genChromaSS.h>

// This filter generates Chroma from an FP16 Luma plane and a U16 RGB plane.
// It also subsamples the data by half in each dimension.
// Makes the assumption that the input data is in the range [0, 0x3fff] !!
//
//Octave ref
//  oo_luma = 1 ./ (presharp + cfg.chroma_epsilon);
//  for i = 1:3
//      ci(:,:,i) = lanczos_downsample(img(:,:,i) .* oo_luma * (255/3));
//  end

//##########################################################################################
#if defined(SIPP_USE_MVCV)
void mvispGenChromaSS_asm(UInt8* outC[3], UInt8** inRGB, int eps, float chromaScale[3], UInt32 width);
#else
void GenChromaSS         (UInt8* outC[3], UInt8** inRGB, int eps, float chromaScale[3], UInt32 width)
{
    UInt8 *inR0   = inRGB[0]; //first line of R
    UInt8 *inG0   = inRGB[1]; //first line of G
    UInt8 *inB0   = inRGB[2]; //first line of B
    UInt8 *inR1   = inRGB[3]; //second line of R
    UInt8 *inG1   = inRGB[4]; //second line of G
    UInt8 *inB1   = inRGB[5]; //second line of B

    UInt8  *outCR  = outC[0];
    UInt8  *outCG  = outC[1];
    UInt8  *outCB  = outC[2];

    UInt32 i;
    half luma;
    half normValHalfR = chromaScale[0] * (255.0f/3.0f);
    half normValHalfG = chromaScale[1] * (255.0f/3.0f);
    half normValHalfB = chromaScale[2] * (255.0f/3.0f);
    half epsilon = (float)eps * 4.0f;

    for(i=0; i<width; i++)
    {
        half inRGB[3];
        half lumaInv;

        inRGB[0] = (half)((float)inR0[i*2] + inR0[i*2+1] + inR1[i*2] + inR1[i*2+1]);
        inRGB[1] = (half)((float)inG0[i*2] + inG0[i*2+1] + inG1[i*2] + inG1[i*2+1]);
        inRGB[2] = (half)((float)inB0[i*2] + inB0[i*2+1] + inB1[i*2] + inB1[i*2+1]);
        luma = epsilon + (inRGB[0] + inRGB[1] + inRGB[1] + inRGB[2]) * 0.25f;
        lumaInv = 1.0f/luma;

        outCR[i] =  (UInt8)((float)((inRGB[0] * lumaInv*normValHalfR) > 255 ? 255 : (inRGB[0] * lumaInv*normValHalfR)));
        outCG[i] =  (UInt8)((float)((inRGB[1] * lumaInv*normValHalfG) > 255 ? 255 : (inRGB[1] * lumaInv*normValHalfG)));
        outCB[i] =  (UInt8)((float)((inRGB[2] * lumaInv*normValHalfB) > 255 ? 255 : (inRGB[2] * lumaInv*normValHalfB)));
    }
}
#endif

//##########################################################################################
void svuGenChromaSS(SippFilter *fptr)
{
    UInt8 *inRGB[6]; //Input RGB lines (parent[0] must be the 3xplane RGB)
    UInt8  *outC[3]; //Output Chroma

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

    #if 0 //slower access
        inRGB[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0); //1st line of R
        inRGB[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1); //1st line of G
        inRGB[2] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2); //1st line of B
        inRGB[3] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0); //2ns line of R
        inRGB[4] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 1); //2ns line of G
        inRGB[5] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 2); //2ns line of B
    #else //faster access
        getIn3PlanePtr(fptr, 0, iterNum, 0, &inRGB[0]);       //RGB ptrs for 1st line
        getIn3PlanePtr(fptr, 0, iterNum, 1, &inRGB[3]);       //RGB ptrs for 2nd line
    #endif

        outC[0] = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outC[1] = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        outC[2] = (UInt8 *)getOutPtr(fptr, iterNum, 2);

    #ifdef SIPP_USE_MVCV
        mvispGenChromaSS_asm(outC, inRGB,
          ((ChrGenSSParam*)fptr->params)->epsilon,
          ((ChrGenSSParam*)fptr->params)->scale, fptr->sliceWidth);
    #else
        GenChromaSS(outC, inRGB,
          ((ChrGenSSParam*)fptr->params)->epsilon,
          ((ChrGenSSParam*)fptr->params)->scale, fptr->sliceWidth);
    #endif
    }
}
