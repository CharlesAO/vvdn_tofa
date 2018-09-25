#include "whiteBalanceRGB.h"

void mvcvWhiteBalanceRGB(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], u16** outR, u16** outG, u16** outB, u16 clamp[1], u32 width) 
{
	int i;	
	u16* rIn = inR[0];
	u16* gIn = inG[0];
	u16* bIn = inB[0];

	u16* rOut = outR[0];
	u16* gOut = outG[0];
	u16* bOut = outB[0];
	
	u32 r,g,b;
	//clamp = 32767;
	for (i = 0; i < (int)width; i++) {
		r = ((u32)rIn[i] * (u32)awbCoef[0]) >> 15;
		g = ((u32)gIn[i] * (u32)awbCoef[1]) >> 15;
		b = ((u32)bIn[i] * (u32)awbCoef[2]) >> 15;
		
	rOut[i] = (u16)(r > clamp[0] ? clamp[0] : r);
	gOut[i] = (u16)(g > clamp[0] ? clamp[0] : g);
	bOut[i] = (u16)(b > clamp[0] ? clamp[0] : b);
	}
}
#ifdef MOVICOMPILE_OPTIMIZED
 void mvcvWhiteBalanceRGB_opt(unsigned short ** __restrict inR, unsigned short ** __restrict inG, unsigned short ** __restrict inB, 
                                                    unsigned short awbCoef[3], unsigned short ** __restrict outR, unsigned short ** __restrict outG, 
                                                    unsigned short ** __restrict outB, unsigned short clamp[1], unsigned int width) {
  short4 * __restrict rIn = (short4 *) inR[0];
  short4 * __restrict gIn = (short4 *) inG[0];
  short4 * __restrict bIn = (short4 *) inB[0];

  short4 * __restrict rOut = (short4 *) outR[0];
  short4 * __restrict gOut = (short4 *) outG[0];
  short4 * __restrict bOut = (short4 *) outB[0];

  int4 clampVal = clamp[0];
  
  int4 awbR = awbCoef[0];
  int4 awbG = awbCoef[1];
  int4 awbB = awbCoef[2];
  
  int4 rIn4 = mvuConvert_int4(rIn[0]);
  int4 gIn4 = mvuConvert_int4(gIn[0]);
  int4 bIn4 = mvuConvert_int4(bIn[0]);
  
  #pragma unroll 16
  for (unsigned int i = 0; i < width/4; ++i) {
    uint4 r = mvuAs_uint4(rIn4 * awbR) >> 15;
    uint4 g = mvuAs_uint4(gIn4 * awbG) >> 15;
    uint4 b = mvuAs_uint4(bIn4 * awbB) >> 15;
    
    rIn4 = mvuConvert_int4(rIn[i+1]);
    gIn4 = mvuConvert_int4(gIn[i+1]);
    bIn4 = mvuConvert_int4(bIn[i+1]);
    
    // This is a slightly faster version of:
    //  rOut[i] = mvuConvert_short4(__builtin_shave_cmu_clamp0_i32_rr_int4(mvuAs_int4(r), clampVal));
    
    short8 rResult = mvuAs_short8(__builtin_shave_cmu_clamp0_i32_rr_int4(mvuAs_int4(r), clampVal));
    short8 gResult = mvuAs_short8(__builtin_shave_cmu_clamp0_i32_rr_int4(mvuAs_int4(g), clampVal));
    short8 bResult = mvuAs_short8(__builtin_shave_cmu_clamp0_i32_rr_int4(mvuAs_int4(b), clampVal));
    
    rOut[i] = __builtin_shufflevector(rResult, rResult, 0, 2, 4, 6, 8, 10, 12, 14).s0123;
    gOut[i] = __builtin_shufflevector(gResult, gResult, 0, 2, 4, 6, 8, 10, 12, 14).s0123;
    bOut[i] = __builtin_shufflevector(bResult, bResult, 0, 2, 4, 6, 8, 10, 12, 14).s0123;
  }
}
#endif
