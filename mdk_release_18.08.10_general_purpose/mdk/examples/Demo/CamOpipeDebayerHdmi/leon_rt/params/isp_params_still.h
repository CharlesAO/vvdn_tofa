// GENERATED TEST FILE (OCTAVE dump) !

//Input image dimensions:
#define ISPC_BAYER_W_STILL_IMX214	4208
#define ISPC_BAYER_H_STILL_IMX214	3120
#define ISPC_BAYER_W_STILL_IMX378	4208
#define ISPC_BAYER_H_STILL_IMX378	3120
#define ISPC_BAYER_BITS_STILL	10

// Demosaic
#define ISPC_D65_A_TL84_DEMOSAIC_MIX_SLOPE       	 1800
#define ISPC_D65_A_TL84_DEMOSAIC_MIX_OFFSET      	 254
#define ISPC_D65_A_TL84_DEMOSAIC_MIX_GRADIENT_MUL	 238
#define ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_RED	 64
#define ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_GREEN	 127
#define ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_BLUE	 64

// Chroma generation
#define ISPC_D65_A_TL84_CGEN_EPSILON           	2
#define ISPC_D65_A_TL84_CGEN_KR                	106
#define ISPC_D65_A_TL84_CGEN_KG                	191
#define ISPC_D65_A_TL84_CGEN_KB                	149
#define ISPC_D65_A_TL84_CGEN_LUMA_COEFF_R      	64
#define ISPC_D65_A_TL84_CGEN_LUMA_COEFF_G      	127
#define ISPC_D65_A_TL84_CGEN_LUMA_COEFF_B      	64
#define ISPC_D65_A_TL84_CGEN_PFR_STRENGTH      	80
#define ISPC_D65_A_TL84_CGEN_DESAT_OFFSET       0
#define ISPC_D65_A_TL84_CGEN_DESAT_SLOPE       	232

// Median
#define ISPC_D65_A_TL84_CHROMA_MEDIAN_SIZE       	7

// Median Mix
#define ISPC_D65_A_TL84_CHROMA_MEDIAN_MIX_SLOPE  	166
#define ISPC_D65_A_TL84_CHROMA_MEDIAN_MIX_OFFSET 	3

// LowPass
extern uint8_t ispcLowpassKernel[9];

// Sharpen
#define ISPC_D65_A_TL84_SHARP_SIGMA           	0.643838
#define ISPC_D65_A_TL84_SHARP_STRENGTH_DARKEN 	1.933594
#define ISPC_D65_A_TL84_SHARP_STRENGTH_LIGHTEN	1.223633
#define ISPC_D65_A_TL84_SHARP_ALPHA           	0.003012
#define ISPC_D65_A_TL84_SHARP_OVERSHOOT       	1.023438
#define ISPC_D65_A_TL84_SHARP_UNDERSHOOT      	0.921387
#define ISPC_D65_A_TL84_SHARP_RANGE_STOP_0    	0.000000
#define ISPC_D65_A_TL84_SHARP_RANGE_STOP_1    	0.000408
#define ISPC_D65_A_TL84_SHARP_RANGE_STOP_2    	0.991699
#define ISPC_D65_A_TL84_SHARP_RANGE_STOP_3    	0.999023
#define ISPC_D65_A_TL84_SHARP_MIN_THR         	0.000375
#define ISPC_D65_A_TL84_SHARP_COEF0           	0.070159
#define ISPC_D65_A_TL84_SHARP_COEF1           	0.131075
#define ISPC_D65_A_TL84_SHARP_COEF2           	0.190713
#define ISPC_D65_A_TL84_SHARP_COEF3           	0.216106

// Luma denoise params:
#define ISPC_D65_A_TL84_LUMA_DNS_STRENGTH	22.750000
#define ISPC_D65_A_TL84_LUMA_DNS_ALPHA   	72	// 8bit value
#define ISPC_D65_A_TL84_LUMA_DNS_GAMMA 0.700000
#define ISPC_D65_A_TL84_LUMA_DNS_ANGLE_OF_VIEW 25.000000

// The following coefs are mirrored by the HW to obtain 7x7 matrix
// The 16 coefs represent the top-left square of a 7x7 symetrical matrix
// Each entry is 2 bits, and represents a left shift applied to
// the weight at the corresponding location.
#define ISPC_D65_A_TL84_LUMA_DNS_F2         0x00000000
#define ISPC_D65_A_TL84_LUMA_DNS_BITPOS     7

extern uint8_t ispcLumaDnsLut[32];

extern uint8_t ispcYDnsDistLut[256];

extern uint8_t ispcGammaLut0_32[9];
extern uint8_t ispcGammaLut32_255[9];

#define ISPC_D65_A_TL84_LUMA_DNS_REF_SHIFT 	15

extern float ispcLtmCurves[16*8];

// Pre-filter for Local Tone Mapping:
#define ISPC_D65_A_TL84_LTM_FILTER_TH1    32

// DoG Denoise params:
#define ISPC_D65_A_TL84_DOG_THR         0
#define ISPC_D65_A_TL84_DOG_STRENGTH    0
#define ISPC_D65_A_TL84_DOG_SIGMA11     1.5
#define ISPC_D65_A_TL84_DOG_SIGMA15     4.5

extern uint8_t dogCoeffs11[6];
extern uint8_t dogCoeffs15[8];

// Chroma denoise params:
#define ISPC_D65_A_TL84_CHROMA_DNS_TH_R    6
#define ISPC_D65_A_TL84_CHROMA_DNS_TH_G    7
#define ISPC_D65_A_TL84_CHROMA_DNS_TH_B    8
#define ISPC_D65_A_TL84_CHROMA_DNS_LIMIT   7
#define ISPC_D65_A_TL84_CHROMA_DNS_H_ENAB  1

// Grey desaturate params:
#define ISPC_D65_A_TL84_GREY_DESAT_OFFSET 0
#define ISPC_D65_A_TL84_GREY_DESAT_SLOPE  203
#define ISPC_D65_A_TL84_GREY_POINT_R      106
#define ISPC_D65_A_TL84_GREY_POINT_G      191
#define ISPC_D65_A_TL84_GREY_POINT_B      149

// Color Combine params:
// "K" co-efficients are in U(4,8) format, so mutliply by 256
#define ISPC_D65_A_TL84_CC_KR      615
#define ISPC_D65_A_TL84_CC_KG      342
#define ISPC_D65_A_TL84_CC_KB      438

extern float ispc_D65_CCM[9];
extern float ispc_A_CCM[9];
extern float ispc_TL84_CCM[9];

extern float ispcCCMOff[3];

extern uint16_t lut3d[16*16*16*4];

extern float ispcGammaTable[8192];
extern float ispcCSC[9];

extern float ispcCSCOff[3];

//Global rescale factors factor = N / D
#define N 1  //5
#define D_IMX214 1  //4
#define D_IMX378 2  //4
extern uint8_t hCoefs[]; //up to 8 phases
extern uint8_t vCoefs[];


