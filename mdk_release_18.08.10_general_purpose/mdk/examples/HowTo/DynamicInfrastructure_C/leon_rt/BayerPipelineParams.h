#include "sippPlatform.h"
//GENERATED TEST FILE (OCTAVE dump) !

//In test file name: 
#define SZ_IN_IMG   "coloscene_f6400k_3264x2448_12ms_1x.raw"

//Test image resolution(s): 
//WARNING: debayer cuts 10 pix on vertical and horizontal 
//         but we lie a bit on horizontal by doing -16 to get aligned output line  
//         This means debayer will write 6 more pixels in padding area (not big deal hopefully) 
#define BAYER_W    522
#define BAYER_H    128

#define IMG_W      (BAYER_W-10)
#define IMG_H      (BAYER_H-10)

#define BAYER_BITS 10

//Black level should not be used really... 
//just use the img dumped after -= Black level
#define BLACK_LEVEL 0.015625 //for data in range [0..1]

//gains are 8.8 fixed point format
#define LSC_GAIN_MAP_W 56
#define LSC_GAIN_MAP_H 44

//Bayer gain map: 
extern UInt16 lscMesh[44 * 56] ALIGNED(8);

//Bad pixel suppression
#define BAD_PIX_ALPHA_G_HOT    0x6    // 4 bits
#define BAD_PIX_ALPHA_RB_HOT   0x6    // 4 bits
#define BAD_PIX_ALPHA_G_COLD   0x6    // 4 bits
#define BAD_PIX_ALPHA_RB_COLD  0x6    // 4 bits
#define BAD_PIX_NOISE_LEVEL    0x0000 //16 bits

//GR-GB imbalance filter
#define GRGB_IMBAL_PLAT_DARK     576
#define GRGB_IMBAL_DECAY_DARK    640
#define GRGB_IMBAL_PLAT_BRIGHT   800
#define GRGB_IMBAL_DECAY_BRIGHT  960
#define GRGB_IMBAL_THRESHOLD     1000  // 8 bits

#define CHROMA_GEN_EPSILON     0.000978
#define CHROMA_MEDIAN_SIZE     3

//AWB digital gains are 8.8 fixed-point format
#define DIGI_GAIN_GR    0x0104>>2  //8.8 bits
#define DIGI_GAIN_GB    0x0104>>2  //8.8 bits
#define DIGI_GAIN_R     0x01ab>>2  //8.8 bits
#define DIGI_GAIN_B     0x01ac>>2  //8.8 bits

#define DIGI_SAT_GR     255    //16 bits
#define DIGI_SAT_GB     255    //16 bits
#define DIGI_SAT_R      255    //16 bits
#define DIGI_SAT_B      255    //16 bits

//Sharpen
#define SHARP_SIGMA        1.300000
#define SHARP_STRENGTH     0x3e00    //fp16 (fp32 value is: 1.500000)
#define SHARP_ALPHA        0x3666    //fp16 (fp32 value is: 0.400000)
#define SHARP_OVERSHOOT    0x3c9a    //fp16 (fp32 value is: 1.150000)
#define SHARP_UNDERSHOOT   0x3af5    //fp16 (fp32 value is: 0.869565)

#define SHARP_RANGE_STOP_0 0x1c04    //fp16 [0..1] (fp32 value is: 1.000000)
#define SHARP_RANGE_STOP_1 0x2d05    //fp16 [0..1] (fp32 value is: 20.000000)
#define SHARP_RANGE_STOP_2 0x3ae7    //fp16 [0..1] (fp32 value is: 220.000000)
#define SHARP_RANGE_STOP_3 0x3bd8    //fp16 [0..1] (fp32 value is: 250.000000)

#define SHARP_MIN_THR      0x1a25    //fp16 (fp32 value is: 0.003000)

//Luma denoise params:
#define LUMA_DNS_STRENGTH  12.000000
#define LUMA_DNS_ALPHA     96 //8bit value

//following coefs are to be mirrored by HW and thus obtain 7x7 window
//the 16 coefs represent the top-left square of a 7x7 symetrical kernel
//each entry is 2 bit, and represents number of shifts 
#define LUMA_DNS_F2         0x55555555

//Luma Denoise reference LUT (weights for distances from the center... )
extern UInt8 yDnsRefLut[256];
extern UInt8 yDnsGammaLut[256];

#define LUMA_DNS_REF_SHIFT      14
#define LUMA_DNS_REF_GAMMA      0.800000

//Luma random noise params:
#define LUMA_RAND_NOISE_STRENGTH 0.080000

//Chroma denoise params:
#define CHROMA_DNS_T1      4
#define CHROMA_DNS_T2      5
#define CHROMA_DNS_H_ENAB  1

//Chroma-block params:
extern float ccm[9];

#define DESAT_T1  0.007843
#define DESAT_MUL 19.615385

//Display Gamma params:
#define GAMMA_DARK       1.700000
#define GAMMA_BRIGHT     2.200000
#define GAMMA_ACE_SUB    0.047059
#define GAMMA_ACE_MUL    1.053719

extern UInt16 sharpen_coeffs[4];
extern UInt16 gammaTable[256] ALIGNED(8);
extern UInt8 lumaDnsLut[32] ALIGNED(8);
extern UInt16 horzCoefs[] ALIGNED(8);
extern UInt16 vertCoefs[] ALIGNED(8);
extern int bitpos;

//2X downscale
#define HORZ_N     1
#define HORZ_D     2

#define VERT_N     1
#define VERT_D     2

#define CCM_COEF(x) (UInt16)(x*1024)
