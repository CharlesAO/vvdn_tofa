// GENERATED TEST FILE (OCTAVE dump) !

//Input image dimensions:
//#define ISPC_BAYER_W_STILL  4208
//#define ISPC_BAYER_H_STILL  3120
#define ISPC_BAYER_BITS_STILL	10

// Demosaic
#define ISPC_DEMOSAIC_MIX_SLOPE          4000 // OK
#define ISPC_DEMOSAIC_MIX_OFFSET          254 // OK
#define ISPC_DEMOSAIC_MIX_GRADIENT_MUL    250 // OK
#define ISPC_DEMOSAIC_LUMA_WEIGHT_RED      77 // OK
#define ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN   150 // OK
#define ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE     29 // OK

// Chroma generation
#define ISPC_CGEN_EPSILON                  2 //OK
#define ISPC_CGEN_KR                     106 //OK
#define ISPC_CGEN_KG                     191 //OK
#define ISPC_CGEN_KB                     149 //OK
#define ISPC_CGEN_LUMA_COEFF_R            77 //OK
#define ISPC_CGEN_LUMA_COEFF_G           150 //OK
#define ISPC_CGEN_LUMA_COEFF_B            29 //OK
#define ISPC_CGEN_PFR_STRENGTH            12 //OK
#define ISPC_CGEN_DESAT_OFFSET             0 //OK
#define ISPC_CGEN_DESAT_SLOPE            203 //OK

// Median
#define ISPC_CHROMA_MEDIAN_SIZE       	7    //OK

// Median Mix
#define ISPC_CHROMA_MEDIAN_MIX_SLOPE  	166 // OK
#define ISPC_CHROMA_MEDIAN_MIX_OFFSET 	  1 // OK

// LowPass
extern uint8_t ispcLowpassKernel[9];

// Sharpen 
#define ISPC_SHARP_SIGMA            1.350000 //float
#define ISPC_SHARP_STRENGTH_DARKEN  0x3f00 //fp16 OK
#define ISPC_SHARP_STRENGTH_LIGHTEN 0x3dcc //fp16 OK
#define ISPC_SHARP_ALPHA            0x289a //fp16 OK
#define ISPC_SHARP_OVERSHOOT        0x3c33 //fp16 OK
#define ISPC_SHARP_UNDERSHOOT       0x3b47 //fp16 OK
#define ISPC_SHARP_RANGE_STOP_0     0x128d //fp16 OK
#define ISPC_SHARP_RANGE_STOP_1     0x128d //fp16 OK
#define ISPC_SHARP_RANGE_STOP_2     0x3b70 //fp16 OK
#define ISPC_SHARP_RANGE_STOP_3     0x3c00 //fp16 OK
#define ISPC_SHARP_MIN_THR          0x1d1d //fp16 OK
#define ISPC_SHARP_COEF0            0x1c8a //fp16 OK
#define ISPC_SHARP_COEF1            0x2ae9 //fp16 OK
#define ISPC_SHARP_COEF2            0x33be //fp16 OK
#define ISPC_SHARP_COEF3            0x3662 //fp16 OK

// Luma denoise params:
#define ISPC_LUMA_DNS_STRENGTH  33.000000  //OK
#define ISPC_LUMA_DNS_ALPHA            74  //OK 8bit value

// The following coefs are mirrored by the HW to obtain 7x7 matrix
// The 16 coefs represent the top-left square of a 7x7 symetrical matrix
// Each entry is 2 bits, and represents a left shift applied to
// the weight at the corresponding location.
#define ISPC_LUMA_DNS_F2         0xe5a45440 //OK
#define ISPC_LUMA_DNS_BITPOS     5          //OK 

extern uint8_t ispcLumaDnsLut [ 32];  //OK
extern uint8_t ispcYDnsDistLut[256];  //OK

extern uint8_t ispcGammaLut0_32  [9]; //OK
extern uint8_t ispcGammaLut32_255[9]; //OK

#define ISPC_LUMA_DNS_REF_SHIFT	15    //OK

extern uint16_t ispcLtmCurves[16*8];  //OK

// Pre-filter for Local Tone Mapping:
#define ISPC_LTM_FILTER_TH1    32 //OK 

// DoG Denoise params:
#define ISPC_DOG_THR         6 //OK 
#define ISPC_DOG_STRENGTH    0 //25 original

extern uint8_t dogCoeffs11[6];
extern uint8_t dogCoeffs15[8];

// Chroma denoise params:
#define ISPC_CHROMA_DNS_TH_R    6  //OK
#define ISPC_CHROMA_DNS_TH_G    7  //OK
#define ISPC_CHROMA_DNS_TH_B    8  //OK
#define ISPC_CHROMA_DNS_LIMIT   7  //OK
#define ISPC_CHROMA_DNS_H_ENAB  1  //OK

// Grey desaturate params:
#define ISPC_GREY_DESAT_OFFSET   0 //OK
#define ISPC_GREY_DESAT_SLOPE  230 //OK
#define ISPC_GREY_POINT_R      106 //OK
#define ISPC_GREY_POINT_G      191 //OK
#define ISPC_GREY_POINT_B      149 //OK

// Color Combine params:
// "K" co-efficients are in U(4,8) format, so mutliply by 256
#define ISPC_CC_KR      618 // OK
#define ISPC_CC_KG      343 // OK
#define ISPC_CC_KB      440 // OK

extern float ispcCCM   [9]; //OK
extern float ispcCCMOff[3]; //OK

extern float ispcCSC   [9]; //OK
extern float ispcCSCOff[3]; //OK
//Global rescale factors factor = N / D
#define N 1  //5
#define D 1  //4
extern uint8_t hCoefs[]; //up to 8 phases
extern uint8_t vCoefs[];
