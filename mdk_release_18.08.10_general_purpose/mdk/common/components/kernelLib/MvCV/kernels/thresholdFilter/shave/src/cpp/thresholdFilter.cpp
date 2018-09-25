#include "thresholdFilter.h"
#include <mvcv_types.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/// ThresholdFilter
///@param[in] cornerScores 	- pointer to the input line
///@param[in] threshold 	- threshold value
///@param[in] width			- input line width
///@param[in] posOffset		- offset value 
///@param[out] candPos 		- pointer to the output line
///@param[out] candCount 	- pointer to the width of the output line


void mvcvThresholdFilter(fp32* cornerScores, fp32 threshold, u32 width, u32 posOffset, u16* candPos, u32* candCount)
{
     unsigned short candidate;

    for (candidate = posOffset; candidate < posOffset + width; candidate++)
    {
        if (cornerScores[candidate] > threshold)
        {
            candPos[*candCount] = candidate;
            *candCount = *candCount + 1;
        }
    }
	return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvThresholdFilter_opt (float * __restrict cornerScores, float threshold, 
                                                     unsigned int width, unsigned int posOffset, 
                                                     unsigned short * __restrict candPos, unsigned int * __restrict candCount) {
  unsigned int originalCandPos = (unsigned int) candPos;
  float4 threshold4 = {threshold, threshold, threshold, threshold};
  UNUSED(threshold4);
  unsigned short cand0 = posOffset;
  unsigned short cand1 = cand0 + 1;
  unsigned short cand2 = cand0 + 2;
  unsigned short cand3 = cand0 + 3;
  
  volatile float * preLoadAddr = cornerScores;
  
  float score0 = preLoadAddr[posOffset];
  float score1 = preLoadAddr[posOffset+1];
  float score2 = preLoadAddr[posOffset+2];
  float score3 = preLoadAddr[posOffset+3];
  
  for (unsigned int candidate = posOffset; candidate < posOffset + (width&0xfffffffc); candidate += 4) {
    if (score0 > threshold)
      *candPos++ = cand0;
    
    if (score1 > threshold)
      *candPos++ = cand1;
    
    if (score2 > threshold)
      *candPos++ = cand2;
    
    if (score3 > threshold)
      *candPos++ = cand3;
    
    score0 = cornerScores[candidate+4];
    score1 = cornerScores[candidate+5];
    score2 = cornerScores[candidate+6];
    score3 = cornerScores[candidate+7];
    
    cand0 += 4; 
    cand1 += 4;
    cand2 += 4;
    cand3 += 4;
  }
  
  *candCount = (((unsigned int) candPos) - originalCandPos)/2;
}
#endif
