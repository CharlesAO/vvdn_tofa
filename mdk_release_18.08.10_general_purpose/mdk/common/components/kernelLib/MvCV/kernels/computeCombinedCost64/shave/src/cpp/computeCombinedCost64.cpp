#include "computeCombinedCost64.h"

void mvcvComputeCombinedCost64(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
{
	u32 disparities = 64;
	u32 result = 0;
	u32 col, disp;

	// compute combined cost for 64 disparities
	for (col = 0; col < width; col++)
	{
		for (disp = 0; disp < disparities; disp++)
		{
			result = (alpha * disparityCost[col * disparities + disp] + beta * adCost[col * disparities + disp]) / normFactor;
			if (result > 255) result = 255;
			disparityCost[col * disparities + disp] = result;
			
		}
	}
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvComputeCombinedCost64_opt(u8* __restrict disparityCost, u8* __restrict adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
{
	u32 disparities = 64;
	UNUSED(disparities);
	half8 result_1, result_2, result_3, result_4;
	half8 result_5, result_6, result_7, result_8;
	
	u32 col;

	float invNF = 1.0 / normFactor;
	half8 alpha1 = alpha * invNF;
	half8 beta1 = beta * invNF;

	uchar8 * __restrict dispCptr = (uchar8 *)disparityCost;
	uchar8 * __restrict adCptr = (uchar8 *)adCost;

	half8 next_disp_1 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr);
	half8 next_ad_1 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr);

	half8 next_disp_2 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 1);
	half8 next_ad_2 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 1);

	half8 next_disp_3 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 2);
	half8 next_ad_3 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 2);

	half8 next_disp_4 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 3);
	half8 next_ad_4 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 3);

	half8 next_disp_5 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 4);
	half8 next_ad_5 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 4);

	half8 next_disp_6 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 5);
	half8 next_ad_6 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 5);

	half8 next_disp_7 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 6);
	half8 next_ad_7 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 6);

	half8 next_disp_8 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 7);
	half8 next_ad_8 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 7);


	// compute combined cost for 64 disparities
	for (col = 0; col < width; col++)
	  {
   	        half8 disp_1 = next_disp_1;     half8 ad_1 = next_ad_1;
		half8 disp_2 = next_disp_2;     half8 ad_2 = next_ad_2;
		half8 disp_3 = next_disp_3;     half8 ad_3 = next_ad_3;
		half8 disp_4 = next_disp_4;     half8 ad_4 = next_ad_4;
		half8 disp_5 = next_disp_5;     half8 ad_5 = next_ad_5;
		half8 disp_6 = next_disp_6;     half8 ad_6 = next_ad_6;
		half8 disp_7 = next_disp_7;     half8 ad_7 = next_ad_7;
		half8 disp_8 = next_disp_8;     half8 ad_8 = next_ad_8;

		next_disp_1 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr+8);
		next_ad_1 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr+8);

		next_disp_2 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 9);
		next_ad_2 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 9);

		next_disp_3 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 10);
		next_ad_3 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 10);

		next_disp_4 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 11);
		next_ad_4 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 11);

		next_disp_5 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 12);
		next_ad_5 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 12);

		next_disp_6 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 13);
		next_ad_6 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 13);

		next_disp_7 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 14);
		next_ad_7 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 14);

		next_disp_8 = __builtin_shave_lsu_ld128_u8f_f16_r(dispCptr + 15);
		next_ad_8 = __builtin_shave_lsu_ld128_u8f_f16_r(adCptr + 15);

		result_1 = (alpha1 * disp_1 + beta1 * ad_1);
		result_2 = (alpha1 * disp_2 + beta1 * ad_2);
		result_3 = (alpha1 * disp_3 + beta1 * ad_3);
		result_4 = (alpha1 * disp_4 + beta1 * ad_4);
		result_5 = (alpha1 * disp_5 + beta1 * ad_5);
		result_6 = (alpha1 * disp_6 + beta1 * ad_6);
		result_7 = (alpha1 * disp_7 + beta1 * ad_7);
		result_8 = (alpha1 * disp_8 + beta1 * ad_8);

		__builtin_shave_lsu_st128_f16_u8f_rr(result_1, dispCptr);
		__builtin_shave_lsu_st128_f16_u8f_rr(result_2, dispCptr + 1);

		__builtin_shave_lsu_st128_f16_u8f_rr(result_3, dispCptr + 2);
		__builtin_shave_lsu_st128_f16_u8f_rr(result_4, dispCptr + 3);

		__builtin_shave_lsu_st128_f16_u8f_rr(result_5, dispCptr + 4);
		__builtin_shave_lsu_st128_f16_u8f_rr(result_6, dispCptr + 5);

		__builtin_shave_lsu_st128_f16_u8f_rr(result_7, dispCptr + 6);
		__builtin_shave_lsu_st128_f16_u8f_rr(result_8, dispCptr + 7);

		dispCptr += 8;
		adCptr += 8;
	}
}
#endif
