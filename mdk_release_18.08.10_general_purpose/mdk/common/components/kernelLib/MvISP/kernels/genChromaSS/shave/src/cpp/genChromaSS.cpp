///
/// @file      
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB 
///            image and sub-sample (re-size) the output image by half 
///             in each dimension
///
#include <mv_types.h>
#include <genChromaSS.h>



void mvispGenChromaSS(unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width)
{
  unsigned char *inR0   = inRGB[0]; //first line of R
  unsigned char *inG0   = inRGB[1]; //first line of G
  unsigned char *inB0   = inRGB[2]; //first line of B
  unsigned char *inR1   = inRGB[3]; //second line of R
  unsigned char *inG1   = inRGB[4]; //second line of G
  unsigned char *inB1   = inRGB[5]; //second line of B
  
  unsigned char  *outCR  = outC[0];
  unsigned char  *outCG  = outC[1];
  unsigned char  *outCB  = outC[2];
  
  unsigned int i;
  half luma;
  half normValHalfR = chromaScale[0] * (255.0f/3.0f);
  half normValHalfG = chromaScale[1] * (255.0f/3.0f);
  half normValHalfB = chromaScale[2] * (255.0f/3.0f);
  half epsilon = (float)eps * 4.0f;

  for(i=0; i<width; i++)
  {
      half inRGB[3];
      half lumaInv;

      inRGB[0] = inR0[i*2] + inR0[i*2+1] + inR1[i*2] + inR1[i*2+1];
      inRGB[1] = inG0[i*2] + inG0[i*2+1] + inG1[i*2] + inG1[i*2+1];
      inRGB[2] = inB0[i*2] + inB0[i*2+1] + inB1[i*2] + inB1[i*2+1];
      luma = epsilon + (inRGB[0] + inRGB[1] + inRGB[1] + inRGB[2]) * 0.25f;
      lumaInv = 1.0/luma;

      outCR[i] =  (inRGB[0] * lumaInv*normValHalfR) > 255 ? 255 : (inRGB[0] * lumaInv*normValHalfR);
      outCG[i] =  (inRGB[1] * lumaInv*normValHalfG) > 255 ? 255 : (inRGB[1] * lumaInv*normValHalfG);
      outCB[i] =  (inRGB[2] * lumaInv*normValHalfB) > 255 ? 255 : (inRGB[2] * lumaInv*normValHalfB);
  }
}



