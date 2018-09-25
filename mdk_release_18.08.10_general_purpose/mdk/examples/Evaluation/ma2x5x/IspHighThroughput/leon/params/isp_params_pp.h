// GENERATED TEST FILE (OCTAVE dump) !

#define ISPC_INPUT_IMAGE  "Studio_D65_800_IMX214_2_002.raw" //OK

//Input image dimensions:
#define ISPC_BAYER_W       4208 //OK
#define ISPC_BAYER_H       3120 //OK
#define ISPC_BAYER_BITS      10 //OK

#define ISPC_BLACK_LEVEL_P0  64 //OK
#define ISPC_BLACK_LEVEL_P1  64 //OK
#define ISPC_BLACK_LEVEL_P2  64 //OK
#define ISPC_BLACK_LEVEL_P3  64 //OK

#define ISPC_SIGMA_NOISE_FLOOR  0 //OK
#define ISPC_SIGMA_THRESH1_P0   0 //OK
#define ISPC_SIGMA_THRESH2_P0   0 //OK
#define ISPC_SIGMA_THRESH1_P1   0 //OK
#define ISPC_SIGMA_THRESH2_P1   0 //OK
#define ISPC_SIGMA_THRESH1_P2   0 //OK
#define ISPC_SIGMA_THRESH2_P2   0 //OK
#define ISPC_SIGMA_THRESH1_P3   0 //OK
#define ISPC_SIGMA_THRESH2_P3   0 //OK

//gains are 8.8 fixed point format
#define ISPC_LSC_GAIN_MAP_W   36 //OK
#define ISPC_LSC_GAIN_MAP_H   28 //OK

extern uint16_t ispcLscMesh[ISPC_LSC_GAIN_MAP_W * ISPC_LSC_GAIN_MAP_H]; //OK

// Bad pixel suppression
#define ISPC_BAD_PIX_ALPHA_G_HOT       0x6  // 4 bits //OK
#define ISPC_BAD_PIX_ALPHA_RB_HOT      0x6  // 4 bits //OK
#define ISPC_BAD_PIX_ALPHA_G_COLD      0x6  // 4 bits //OK
#define ISPC_BAD_PIX_ALPHA_RB_COLD     0x6  // 4 bits //OK
#define ISPC_BAD_PIX_NOISE_LEVEL    0x0000  //16 bits //OK

// GR-GB imbalance filter
#define ISPC_GRGB_IMBAL_PLAT_DARK     0x005f          // OK
#define ISPC_GRGB_IMBAL_DECAY_DARK    0x0096          // OK
#define ISPC_GRGB_IMBAL_PLAT_BRIGHT   0x02b2          // OK
#define ISPC_GRGB_IMBAL_DECAY_BRIGHT  0x02db          // OK
#define ISPC_GRGB_IMBAL_THRESHOLD       0x10// 8 bits // OK

#define ISPC_RAW_GAIN_0     0x0221 // 16 bits //OK
#define ISPC_RAW_GAIN_1     0x0111 // 16 bits //OK
#define ISPC_RAW_GAIN_2     0x0111 // 16 bits //OK
#define ISPC_RAW_GAIN_3     0x01c1 // 16 bits //OK

#define ISPC_RAW_CLAMP_0    1023 // 16 bits //OK
#define ISPC_RAW_CLAMP_1    1023 // 16 bits //OK
#define ISPC_RAW_CLAMP_2    1023 // 16 bits //OK
#define ISPC_RAW_CLAMP_3    1023 // 16 bits //OK

#define ISPC_RAW_OUTPUT_BITS  10 //OK

