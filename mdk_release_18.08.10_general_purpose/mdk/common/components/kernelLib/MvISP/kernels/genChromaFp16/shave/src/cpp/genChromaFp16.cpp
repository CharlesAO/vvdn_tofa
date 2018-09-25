///
/// @file      genChromaFp16.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
///
#include <mv_types.h>
#include "genChromaFp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char clampU8(float in)
{
    in += 0.5; //round to neareset int
    if(in< 0)  { return 0; }
    if(in>255) { return 255; }
               return (unsigned char)in;
}

void mvispGenChromaFp16(unsigned char* outC[3], unsigned short* inRGB[3], half *inY, float eps, float normVal, unsigned int width)
{
  unsigned short *inR   = inRGB[0];
  unsigned short *inG   = inRGB[1];
  unsigned short *inB   = inRGB[2];
  
  unsigned char  *outCR = outC[0];
  unsigned char  *outCG = outC[1];
  unsigned char  *outCB = outC[2];
  
  unsigned int i;
  half ooLuma;
  
  half epsHalf = (half)eps;
  half normValHalf = (half)normVal;

  for(i=0; i<width; i++)
  {  //div oo_luma by extra amount, to scale from input #bits to 8bit output
      half inRGB[3];
      half ooLumaDiv;

      ooLuma   =  (inY[i] + epsHalf);
      inRGB[0] = inR[i];
      inRGB[1] = inG[i];
      inRGB[2] = inB[i];
      ooLumaDiv = normValHalf/ooLuma;

      outCR[i] =  clampU8(inRGB[0] * ooLumaDiv);
      outCG[i] =  clampU8(inRGB[1] * ooLumaDiv);
      outCB[i] =  clampU8(inRGB[2] * ooLumaDiv);
  }
}
