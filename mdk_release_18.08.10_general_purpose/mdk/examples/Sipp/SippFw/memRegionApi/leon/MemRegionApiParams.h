// Test params taken from testFragrakIspVcsParams.h

//Test image resolution(s):
//WARNING: debayer cuts 10 pix on vertical and horizontal
//         but we lie a bit on horizontal by doing -16 to get aligned output line
//         This means debayer will write 6 more pixels in padding area (not big deal hopefully)
  #define MEM_API_TEST_BAYER_WIDTH     522
  #define MEM_API_TEST_BAYER_HEIGHT    128

  #define BAYER_BITS 10

//Gains are 16 fixed point format
  #define LSC_GAIN_MAP_W 58
  #define LSC_GAIN_MAP_H 44

//Bayer gain map:
extern UInt16 lscMesh[44 * 58];

////////////////////////////////////////////////////
// Raw filter setup macros

//Bad pixel suppression
  #define BAD_PIX_ALPHA_G_HOT    0x6    // 4 bits
  #define BAD_PIX_ALPHA_RB_HOT   0x6    // 4 bits
  #define BAD_PIX_ALPHA_G_COLD   0x6    // 4 bits
  #define BAD_PIX_ALPHA_RB_COLD  0x6    // 4 bits
  #define BAD_PIX_NOISE_LEVEL    0x00a3 //16 bits


//GR-GB imbalance filter
  #define GRGB_IMBAL_PLAT_DARK     576
  #define GRGB_IMBAL_DECAY_DARK    640
  #define GRGB_IMBAL_PLAT_BRIGHT   800
  #define GRGB_IMBAL_DECAY_BRIGHT  960
  #define GRGB_IMBAL_THRESHOLD     1000  // 8 bits

#define BAYER_PATTERN GRBG

//AWB digital gains are 16 fixed-point format
  #define DIGI_GAIN_GR    0x0104  // 16 bits
  #define DIGI_GAIN_GB    0x0104  // 16 bits
  #define DIGI_GAIN_R     0x0104  // 16 bits
  #define DIGI_GAIN_B     0x0104  // 16 bits

  #define DIGI_SAT_GR     1023    // 16 bits
  #define DIGI_SAT_GB     1023    // 16 bits
  #define DIGI_SAT_R      1023    // 16 bits
  #define DIGI_SAT_B      1023    // 16 bits

