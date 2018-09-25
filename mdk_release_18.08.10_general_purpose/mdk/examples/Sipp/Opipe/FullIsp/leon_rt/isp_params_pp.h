///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     isp_params_pp.h
///


// 1: Includes
// ----------------------------------------------------------------------------
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// GENERATED TEST FILE (OCTAVE dump) !

#define ISPC_INPUT_IMAGE	"Scene_2800K_5L_1_10bit.raw"

//Input image dimensions:
#define ISPC_BAYER_W	4208
#define ISPC_BAYER_H	3120
#define ISPC_BAYER_BITS	10

#define ISPC_BLACK_LEVEL_P0	16
#define ISPC_BLACK_LEVEL_P1	16
#define ISPC_BLACK_LEVEL_P2	16
#define ISPC_BLACK_LEVEL_P3	16

#define ISPC_SIGMA_NOISE_FLOOR	51
#define ISPC_SIGMA_THRESH1_P0	9
#define ISPC_SIGMA_THRESH2_P0	18
#define ISPC_SIGMA_THRESH1_P1	9
#define ISPC_SIGMA_THRESH2_P1	18
#define ISPC_SIGMA_THRESH1_P2	9
#define ISPC_SIGMA_THRESH2_P2	18
#define ISPC_SIGMA_THRESH1_P3	9
#define ISPC_SIGMA_THRESH2_P3	18

//gains are 8.8 fixed point format
#define ISPC_LSC_GAIN_MAP_W	60
#define ISPC_LSC_GAIN_MAP_H	44

extern uint16_t ispcLscMesh[ISPC_LSC_GAIN_MAP_W * ISPC_LSC_GAIN_MAP_H];

// Bad pixel suppression
#define ISPC_BAD_PIX_ALPHA_G_HOT	0x5	// 4 bits
#define ISPC_BAD_PIX_ALPHA_RB_HOT	0x2	// 4 bits
#define ISPC_BAD_PIX_ALPHA_G_COLD	0x5	// 4 bits
#define ISPC_BAD_PIX_ALPHA_RB_COLD	0x5	// 4 bits
#define ISPC_BAD_PIX_NOISE_LEVEL	0x00a3	//16 bits

// GR-GB imbalance filter
#define ISPC_GRGB_IMBAL_PLAT_DARK	 150
#define ISPC_GRGB_IMBAL_DECAY_DARK	 200
#define ISPC_GRGB_IMBAL_PLAT_BRIGHT	 400
#define ISPC_GRGB_IMBAL_DECAY_BRIGHT	 500
#define ISPC_GRGB_IMBAL_THRESHOLD	  16	// 8 bits

#define ISPC_RAW_GAIN_0		1148	// 16 bits
#define ISPC_RAW_GAIN_1		988	// 16 bits
#define ISPC_RAW_GAIN_2		988	// 16 bits
#define ISPC_RAW_GAIN_3		2273	// 16 bits

#define ISPC_RAW_CLAMP_0	1023	// 16 bits
#define ISPC_RAW_CLAMP_1	1023	// 16 bits
#define ISPC_RAW_CLAMP_2	1023	// 16 bits
#define ISPC_RAW_CLAMP_3	1023	// 16 bits

#define ISPC_RAW_OUTPUT_BITS	10

// 3: Static Local Data
// ----------------------------------------------------------------------------
