#ifndef __ISP_PARAMS_H__
#define __ISP_PARAMS_H__


#define ISPC_BAYER_W   640
#define ISPC_BAYER_H   480

#define RAW_OUT_BITS   8

#define MONO_PIPE_INPUT_BUFFER_WIDTH   ISPC_BAYER_W
#define MONO_PIPE_INPUT_BUFFER_HEIGHT  ISPC_BAYER_H

#ifndef ALIGNED
#define ALIGNED(x) __attribute__((aligned(x)))
#endif

#define BAYER_PATTERN 0		// Don't care, we're doing PLANAR

#define ISPC_BAYER_BITS			10
#define FORMAT          		PLANAR
#define SIGMA_DNS_PASSTHRU_BIT  ENABLED

#define ISPC_BLACK_LEVEL_P0		0
#define ISPC_BLACK_LEVEL_P1		0
#define ISPC_BLACK_LEVEL_P2		0
#define ISPC_BLACK_LEVEL_P3		0

#define ISPC_SIGMA_NOISE_FLOOR	0x0
#define ISPC_SIGMA_THRESH1_P0   0x0
#define ISPC_SIGMA_THRESH2_P0   0x0
#define ISPC_SIGMA_THRESH1_P1   0x0
#define ISPC_SIGMA_THRESH2_P1   0x0
#define ISPC_SIGMA_THRESH1_P2   0x0
#define ISPC_SIGMA_THRESH2_P2   0x0
#define ISPC_SIGMA_THRESH1_P3   0x0
#define ISPC_SIGMA_THRESH2_P3   0x0

//gains are 8.8 fixed point format
#define ISPC_LSC_GAIN_MAP_W   8
#define ISPC_LSC_GAIN_MAP_H   8

// Bad pixel suppression
#define GRGB_IMBAL_PLAT_DARK     	0x0
#define GRGB_IMBAL_DECAY_DARK    	0x0
#define GRGB_IMBAL_PLAT_BRIGHT   	0x0
#define GRGB_IMBAL_DECAY_BRIGHT     0x0
#define ISPC_GRGB_IMBAL_THRESHOLD   0

#define ISPC_BAD_PIX_ALPHA_G_HOT	0x0	// 4 bits
#define ISPC_BAD_PIX_ALPHA_RB_HOT	0x0	// 4 bits
#define ISPC_BAD_PIX_ALPHA_G_COLD	0x0	// 4 bits
#define ISPC_BAD_PIX_ALPHA_RB_COLD	0x0	// 4 bits
#define ISPC_BAD_PIX_NOISE_LEVEL	0x0	//16 bits

#define GRGB_IMBAL_EN            DISABLED
#define AE_PATCH_STATS_EN        DISABLED
#define AF_PATCH_STATS_EN        DISABLED
#define AE_Y_HIST_STATS_EN       DISABLED
#define AE_RGB_HIST_STATS_EN     DISABLED
#define HOT_COLD_PIX_SUPPRESS_EN DISABLED
#define BAYER_2x2_MODE           0

//AWB digital gains are 8.8 fixed-point format
#define DIGI_GAIN_GR    0x0100>>2  //8.8 bits
#define DIGI_GAIN_GB    0x0100>>2  //8.8 bits
#define DIGI_GAIN_R     0x0100>>2  //8.8 bits
#define DIGI_GAIN_B     0x0100>>2  //8.8 bits

#define DIGI_SAT_GR     ((1<<RAW_OUT_BITS)-1)    //16 bits
#define DIGI_SAT_GB     ((1<<RAW_OUT_BITS)-1)    //16 bits
#define DIGI_SAT_R      ((1<<RAW_OUT_BITS)-1)    //16 bits
#define DIGI_SAT_B      ((1<<RAW_OUT_BITS)-1)    //16 bits

// Sharpen
#define SHARPENING 		0
#define BLURRING   		1

#define SHARPEN_MODE    SHARPENING
#define KERNEL_SIZE     7
#define CLAMP_OUTPUT    1
#define OUTPUT_DELTAS   0

#define ISPC_SHARP_STRENGTH_DARKEN 	0x0000
#define ISPC_SHARP_STRENGTH_LIGHTEN	0x0000
#define ISPC_SHARP_ALPHA           	0x0000
#define ISPC_SHARP_OVERSHOOT       	0x4000
#define ISPC_SHARP_UNDERSHOOT      	0x0000
#define ISPC_SHARP_RANGE_STOP_0    	0x0000
#define ISPC_SHARP_RANGE_STOP_1    	0x0000
#define ISPC_SHARP_RANGE_STOP_2    	0x0000
#define ISPC_SHARP_RANGE_STOP_3    	0x0000
#define ISPC_SHARP_MIN_THR         	0x0000
#define ISPC_SHARP_COEF0           	0x0000
#define ISPC_SHARP_COEF1           	0x0000
#define ISPC_SHARP_COEF2           	0x0000
#define ISPC_SHARP_COEF3           	0x0000

// Luma denoise params:
#define ISPC_LUMA_DNS_STRENGTH	150.000000
#define ISPC_LUMA_DNS_ALPHA   	128	// 8bit value

// The following coefs are mirrored by the HW to obtain 7x7 matrix
// The 16 coefs represent the top-left square of a 7x7 symetrical matrix
// Each entry is 2 bits, and represents a left shift applied to
// the weight at the corresponding location.
#define ISPC_LUMA_DNS_F2         0xaaaaa9a4
#define ISPC_LUMA_DNS_BITPOS     7

#define ISPC_LUMA_DNS_REF_SHIFT	 15
#define LUMA_DNS_X_OFFSET        0
#define LUMA_DNS_Y_OFFSET        0

extern u32 gaussLut[4];
extern u32 gammaLut[5];

// Lut
#define FP16_MODE     1
#define CHANNEL_MODE  0
#define NUM_LUTS      1

extern uint8_t ispcLumaDnsLut    [32];
extern uint8_t ispcYDnsDistLut   [256];
extern uint8_t ispcGammaLut0_32  [9];
extern uint8_t ispcGammaLut32_255[9];

extern uint16_t ispcLscMesh[ISPC_LSC_GAIN_MAP_W * ISPC_LSC_GAIN_MAP_H];
extern uint16_t gammaLutFp16[512];

#endif
