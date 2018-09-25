///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     isp_params_still.h
///

// 1: Includes
// ----------------------------------------------------------------------------
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// GENERATED TEST FILE (OCTAVE dump) !

//Input image dimensions:
#define ISPC_BAYER_W_STILL	4208
#define ISPC_BAYER_H_STILL	3120
#define ISPC_BAYER_BITS_STILL	10

// Demosaic
#define ISPC_DEMOSAIC_MIX_SLOPE       	   0
#define ISPC_DEMOSAIC_MIX_OFFSET      	   0
#define ISPC_DEMOSAIC_MIX_GRADIENT_MUL	   0
#define ISPC_DEMOSAIC_LUMA_WEIGHT_RED	  77
#define ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN	 150
#define ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE	  29

// Chroma generation
#define ISPC_CGEN_EPSILON           	1
#define ISPC_CGEN_KR                	106
#define ISPC_CGEN_KG                	191
#define ISPC_CGEN_KB                	149
#define ISPC_CGEN_LUMA_COEFF_R      	76
#define ISPC_CGEN_LUMA_COEFF_G      	150
#define ISPC_CGEN_LUMA_COEFF_B      	29
#define ISPC_CGEN_PFR_STRENGTH      	80
#define ISPC_CGEN_DESAT_OFFSET      	4
#define ISPC_CGEN_DESAT_SLOPE       	43

// Median
#define ISPC_CHROMA_MEDIAN_SIZE       	5

// Median Mix
#define ISPC_CHROMA_MEDIAN_MIX_SLOPE  	10
#define ISPC_CHROMA_MEDIAN_MIX_OFFSET 	-26

// LowPass
extern uint8_t ispcLowpassKernel[9];

// Sharpen 
#define ISPC_SHARP_SIGMA           	2.000000
#define ISPC_SHARP_STRENGTH_DARKEN 	0.800000
#define ISPC_SHARP_STRENGTH_LIGHTEN	2.000000
#define ISPC_SHARP_ALPHA           	0.700000
#define ISPC_SHARP_OVERSHOOT       	1.050000
#define ISPC_SHARP_UNDERSHOOT      	1.000000
#define ISPC_SHARP_RANGE_STOP_0    	0.003922
#define ISPC_SHARP_RANGE_STOP_1    	0.019608
#define ISPC_SHARP_RANGE_STOP_2    	0.980392
#define ISPC_SHARP_RANGE_STOP_3    	1.000000
#define ISPC_SHARP_MIN_THR         	0.000000
#define ISPC_SHARP_COEF0           	0.070159
#define ISPC_SHARP_COEF1           	0.131075
#define ISPC_SHARP_COEF2           	0.190713
#define ISPC_SHARP_COEF3           	0.216106

// Luma denoise params:
#define ISPC_LUMA_DNS_STRENGTH	150.000000
#define ISPC_LUMA_DNS_ALPHA   	128	// 8bit value

// The following coefs are mirrored by the HW to obtain 7x7 matrix
// The 16 coefs represent the top-left square of a 7x7 symetrical matrix
// Each entry is 2 bits, and represents a left shift applied to
// the weight at the corresponding location.
#define ISPC_LUMA_DNS_F2         0xaaaaa9a4
#define ISPC_LUMA_DNS_BITPOS     7

extern uint8_t ispcLumaDnsLut[32];

extern uint8_t ispcYDnsDistLut[256];

extern uint8_t ispcGammaLut0_32[9];
extern uint8_t ispcGammaLut32_255[9];

#define ISPC_LUMA_DNS_REF_SHIFT	15

extern float ispcLtmCurves[16*8];

// Pre-filter for Local Tone Mapping:
#define ISPC_LTM_FILTER_TH1    35 

// DoG Denoise params:
#define ISPC_DOG_THR         4 
#define ISPC_DOG_STRENGTH    191 

extern uint8_t dogCoeffs11[6];
extern uint8_t dogCoeffs15[8];

// Chroma denoise params:
#define ISPC_CHROMA_DNS_TH_R    20 
#define ISPC_CHROMA_DNS_TH_G    15 
#define ISPC_CHROMA_DNS_TH_B    38 
#define ISPC_CHROMA_DNS_LIMIT   100 
#define ISPC_CHROMA_DNS_H_ENAB  7 

// Grey desaturate params:
#define ISPC_GREY_DESAT_OFFSET -35
#define ISPC_GREY_DESAT_SLOPE  7
#define ISPC_GREY_POINT_R      106
#define ISPC_GREY_POINT_G      137
#define ISPC_GREY_POINT_B      75

// Color Combine params:
// "K" co-efficients are in U(4,8) format, so mutliply by 256
#define ISPC_CC_KR      618
#define ISPC_CC_KG      343
#define ISPC_CC_KB      440
extern float ispcCCM[9];

extern float ispcCCMOff[3];

extern uint16_t lut3d[16*16*16*4];

extern float ispcGammaTable[8192];
extern float ispcCSC[9];

extern float ispcCSCOff[3];

// 3: Static Local Data
// ----------------------------------------------------------------------------
#define N 1
#define D 1
extern uint8_t hCoefs[]; //up to 8 phases
extern uint8_t vCoefs[];

