#include <sipp.h>
#include <sippTestCommon.h>
#include "app_config.h"
#include <isp_params.h> //ALIGNED macro
#include <stdint.h>
#include "monoPipe.h"

// Bayer gain map (8.8)
//(dummy passthrough config)
uint16_t ispcLscMesh[ISPC_LSC_GAIN_MAP_W * ISPC_LSC_GAIN_MAP_H] __attribute__((section(".cmx.data"))) __attribute__((aligned(8))) =
{
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
};

uint8_t ispcLumaDnsLut[32] = {
   15, 15, 14, 14, 13, 12, 10, 9, 8, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Luma Denoise reference LUT (weights for distances from the center... )
uint8_t ispcYDnsDistLut[256]  __attribute__((aligned(8))) = {
  0xff, 0xfe, 0xfc, 0xfb, 0xf9, 0xf8, 0xf6, 0xf5,
  0xf3, 0xf2, 0xf0, 0xef, 0xed, 0xec, 0xeb, 0xe9,
  0xe8, 0xe6, 0xe5, 0xe4, 0xe2, 0xe1, 0xe0, 0xde,
  0xdd, 0xdc, 0xda, 0xd9, 0xd8, 0xd6, 0xd5, 0xd4,
  0xd3, 0xd1, 0xd0, 0xcf, 0xcd, 0xcc, 0xcb, 0xca,
  0xc8, 0xc7, 0xc6, 0xc5, 0xc4, 0xc2, 0xc1, 0xc0,
  0xbf, 0xbe, 0xbd, 0xbb, 0xba, 0xb9, 0xb8, 0xb7,
  0xb6, 0xb4, 0xb3, 0xb2, 0xb1, 0xb0, 0xaf, 0xae,
  0xad, 0xac, 0xab, 0xa9, 0xa8, 0xa7, 0xa6, 0xa5,
  0xa4, 0xa3, 0xa2, 0xa1, 0xa0, 0x9f, 0x9e, 0x9d,
  0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96, 0x95,
  0x94, 0x93, 0x92, 0x91, 0x90, 0x8f, 0x8f, 0x8e,
  0x8d, 0x8c, 0x8b, 0x8a, 0x89, 0x88, 0x87, 0x86,
  0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80, 0x7f,
  0x7f, 0x7e, 0x7d, 0x7c, 0x7b, 0x7b, 0x7a, 0x79,
  0x78, 0x77, 0x76, 0x76, 0x75, 0x74, 0x73, 0x73,
  0x72, 0x71, 0x70, 0x6f, 0x6f, 0x6e, 0x6d, 0x6c,
  0x6c, 0x6b, 0x6a, 0x6a, 0x69, 0x68, 0x67, 0x67,
  0x66, 0x65, 0x65, 0x64, 0x63, 0x63, 0x62, 0x61,
  0x60, 0x60, 0x5f, 0x5e, 0x5e, 0x5d, 0x5d, 0x5c,
  0x5b, 0x5b, 0x5a, 0x59, 0x59, 0x58, 0x57, 0x57,
  0x56, 0x56, 0x55, 0x54, 0x54, 0x53, 0x53, 0x52,
  0x51, 0x51, 0x50, 0x50, 0x4f, 0x4f, 0x4e, 0x4d,
  0x4d, 0x4c, 0x4c, 0x4b, 0x4b, 0x4a, 0x4a, 0x49,
  0x49, 0x48, 0x47, 0x47, 0x46, 0x46, 0x45, 0x45,
  0x44, 0x44, 0x43, 0x43, 0x42, 0x42, 0x41, 0x41,
  0x40, 0x40, 0x3f, 0x3f, 0x3e, 0x3e, 0x3e, 0x3d,
  0x3d, 0x3c, 0x3c, 0x3b, 0x3b, 0x3a, 0x3a, 0x39,
  0x39, 0x39, 0x38, 0x38, 0x37, 0x37, 0x36, 0x36,
  0x36, 0x35, 0x35, 0x34, 0x34, 0x34, 0x33, 0x33,
  0x32, 0x32, 0x32, 0x31, 0x31, 0x30, 0x30, 0x30,
  0x2f, 0x2f, 0x2f, 0x2e, 0x2e, 0x2d, 0x2d, 0x2d,
};

uint8_t ispcGammaLut0_32[9] = {
   0, 26, 38, 47, 56, 63, 70, 76, 81,
};

uint8_t ispcGammaLut32_255[9] = {
   0, 81, 119, 149, 175, 197, 218, 237, 255,
};

//Dumped from:
// http://dub30/svn/TRUNK/fragrak_v2/data/fragrak/sipp/MODELS/sipp_hw/octave_compare/luttest/test.cpp
//fp16 channel mode LUT
uint16_t gammaLutFp16[512] __attribute__((section(".cmx.data"))) __attribute__((aligned(8))) =
{
 #if 1 //Octave curve
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0591,0x0844,0x09bf,0x0b3b,0x0c5b,0x0d19,0x0dd6,0x0e94,0x0f52,0x1007,0x1066,0x10c5,0x1124,
  0x1183,0x11e2,0x1241,0x12a0,0x12fe,0x135d,0x13bc,0x140d,0x143d,0x146c,0x149c,0x14cb,0x14fa,0x152a,0x1559,0x1589,
  0x15b8,0x16f1,0x18c7,0x1a16,0x1b65,0x1c5a,0x1d02,0x1da9,0x1e51,0x1ef8,0x1fa0,0x2023,0x2077,0x20cb,0x211f,0x2173,
  0x21c6,0x2225,0x229c,0x2312,0x2389,0x2400,0x243b,0x2476,0x24b1,0x24ed,0x2528,0x2563,0x259f,0x25da,0x2615,0x2651,
  0x268c,0x2706,0x2783,0x27ff,0x283e,0x287c,0x28ba,0x28f8,0x2936,0x2976,0x29b7,0x29f8,0x2a38,0x2a79,0x2aba,0x2afa,
  0x2b3b,0x2b7d,0x2bc0,0x2c01,0x2c22,0x2c44,0x2c65,0x2c87,0x2ca8,0x2cca,0x2cec,0x2d0e,0x2d30,0x2d53,0x2d75,0x2d97,
  0x2db9,0x2dff,0x2e44,0x2e8a,0x2ed0,0x2f16,0x2f5c,0x2fa3,0x2fe9,0x3018,0x303b,0x305f,0x3082,0x30a5,0x30c9,0x30ec,
  0x3110,0x3133,0x3156,0x3179,0x319c,0x31bf,0x31e2,0x3205,0x3228,0x324a,0x326c,0x328e,0x32b0,0x32d2,0x32f3,0x3314,
  0x3335,0x3376,0x33b6,0x33f5,0x3419,0x3437,0x3454,0x3470,0x348c,0x34a4,0x34ba,0x34d3,0x34ec,0x3504,0x351d,0x3536,
  0x354e,0x3566,0x357e,0x3596,0x35ae,0x35c6,0x35dd,0x35f4,0x360c,0x3623,0x3639,0x3650,0x3666,0x367d,0x3693,0x36a9,
  0x36be,0x36e9,0x3713,0x373b,0x3763,0x378a,0x37b0,0x37d5,0x37f8,0x380d,0x381e,0x382e,0x383d,0x384c,0x385a,0x386c,
  0x3879,0x3886,0x3892,0x389e,0x38aa,0x38b6,0x38c2,0x38ce,0x38da,0x38e5,0x38f1,0x38fc,0x3907,0x3912,0x391d,0x3928,
  0x3932,0x3947,0x395c,0x396f,0x3983,0x3995,0x39a7,0x39b9,0x39c9,0x39da,0x39e9,0x39f8,0x3a06,0x3a13,0x3a20,0x3a2c,
  0x3a37,0x3a41,0x3a4b,0x3a52,0x3a5b,0x3a63,0x3a6c,0x3a74,0x3a7c,0x3a84,0x3a8c,0x3a94,0x3a9c,0x3aa3,0x3aab,0x3ab2,
  0x3aba,0x3ac8,0x3ad6,0x3ae3,0x3af1,0x3afd,0x3b0a,0x3b16,0x3b22,0x3b2d,0x3b39,0x3b44,0x3b4e,0x3b59,0x3b63,0x3b6d,
  0x3b77,0x3b80,0x3b8a,0x3b93,0x3b9c,0x3ba5,0x3bad,0x3bb6,0x3bbe,0x3bc7,0x3bcf,0x3bd7,0x3bdf,0x3be8,0x3bf0,0x3bf8,
  0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,
  0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,0x3c00,
#else //  GAMMA = 1.0 (passthrough)
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0400,0x0420,0x0440,0x0460,0x0480,0x04a0,0x04c0,0x04e0,0x0500,0x0520,0x0540,0x0560,0x0580,0x05a0,0x05c0,0x05e0,
  0x0600,0x0620,0x0640,0x0660,0x0680,0x06a0,0x06c0,0x06e0,0x0700,0x0720,0x0740,0x0760,0x0780,0x07a0,0x07c0,0x07e0,
  0x0800,0x0820,0x0840,0x0860,0x0880,0x08a0,0x08c0,0x08e0,0x0900,0x0920,0x0940,0x0960,0x0980,0x09a0,0x09c0,0x09e0,
  0x0a00,0x0a20,0x0a40,0x0a60,0x0a80,0x0aa0,0x0ac0,0x0ae0,0x0b00,0x0b20,0x0b40,0x0b60,0x0b80,0x0ba0,0x0bc0,0x0be0,
  0x0c00,0x0c20,0x0c40,0x0c60,0x0c80,0x0ca0,0x0cc0,0x0ce0,0x0d00,0x0d20,0x0d40,0x0d60,0x0d80,0x0da0,0x0dc0,0x0de0,
  0x0e00,0x0e20,0x0e40,0x0e60,0x0e80,0x0ea0,0x0ec0,0x0ee0,0x0f00,0x0f20,0x0f40,0x0f60,0x0f80,0x0fa0,0x0fc0,0x0fe0,
  0x1000,0x1020,0x1040,0x1060,0x1080,0x10a0,0x10c0,0x10e0,0x1100,0x1120,0x1140,0x1160,0x1180,0x11a0,0x11c0,0x11e0,
  0x1200,0x1220,0x1240,0x1260,0x1280,0x12a0,0x12c0,0x12e0,0x1300,0x1320,0x1340,0x1360,0x1380,0x13a0,0x13c0,0x13e0,
  0x1400,0x1420,0x1440,0x1460,0x1480,0x14a0,0x14c0,0x14e0,0x1500,0x1520,0x1540,0x1560,0x1580,0x15a0,0x15c0,0x15e0,
  0x1600,0x1620,0x1640,0x1660,0x1680,0x16a0,0x16c0,0x16e0,0x1700,0x1720,0x1740,0x1760,0x1780,0x17a0,0x17c0,0x17e0,
  0x1800,0x1820,0x1840,0x1860,0x1880,0x18a0,0x18c0,0x18e0,0x1900,0x1920,0x1940,0x1960,0x1980,0x19a0,0x19c0,0x19e0,
  0x1a00,0x1a20,0x1a40,0x1a60,0x1a80,0x1aa0,0x1ac0,0x1ae0,0x1b00,0x1b20,0x1b40,0x1b60,0x1b80,0x1ba0,0x1bc0,0x1be0,
  0x1c00,0x1c20,0x1c40,0x1c60,0x1c80,0x1ca0,0x1cc0,0x1ce0,0x1d00,0x1d20,0x1d40,0x1d60,0x1d80,0x1da0,0x1dc0,0x1de0,
  0x1e00,0x1e20,0x1e40,0x1e60,0x1e80,0x1ea0,0x1ec0,0x1ee0,0x1f00,0x1f20,0x1f40,0x1f60,0x1f80,0x1fa0,0x1fc0,0x1fe0,
  0x2000,0x2020,0x2040,0x2060,0x2080,0x20a0,0x20c0,0x20e0,0x2100,0x2120,0x2140,0x2160,0x2180,0x21a0,0x21c0,0x21e0,
  0x2200,0x2220,0x2240,0x2260,0x2280,0x22a0,0x22c0,0x22e0,0x2300,0x2320,0x2340,0x2360,0x2380,0x23a0,0x23c0,0x23e0,
  0x2400,0x2420,0x2440,0x2460,0x2480,0x24a0,0x24c0,0x24e0,0x2500,0x2520,0x2540,0x2560,0x2580,0x25a0,0x25c0,0x25e0,
  0x2600,0x2620,0x2640,0x2660,0x2680,0x26a0,0x26c0,0x26e0,0x2700,0x2720,0x2740,0x2760,0x2780,0x27a0,0x27c0,0x27e0,
  0x2800,0x2820,0x2840,0x2860,0x2880,0x28a0,0x28c0,0x28e0,0x2900,0x2920,0x2940,0x2960,0x2980,0x29a0,0x29c0,0x29e0,
  0x2a00,0x2a20,0x2a40,0x2a60,0x2a80,0x2aa0,0x2ac0,0x2ae0,0x2b00,0x2b20,0x2b40,0x2b60,0x2b80,0x2ba0,0x2bc0,0x2be0,
  0x2c00,0x2c20,0x2c40,0x2c60,0x2c80,0x2ca0,0x2cc0,0x2ce0,0x2d00,0x2d20,0x2d40,0x2d60,0x2d80,0x2da0,0x2dc0,0x2de0,
  0x2e00,0x2e20,0x2e40,0x2e60,0x2e80,0x2ea0,0x2ec0,0x2ee0,0x2f00,0x2f20,0x2f40,0x2f60,0x2f80,0x2fa0,0x2fc0,0x2fe0,
  0x3000,0x3020,0x3040,0x3060,0x3080,0x30a0,0x30c0,0x30e0,0x3100,0x3120,0x3140,0x3160,0x3180,0x31a0,0x31c0,0x31e0,
  0x3200,0x3220,0x3240,0x3260,0x3280,0x32a0,0x32c0,0x32e0,0x3300,0x3320,0x3340,0x3360,0x3380,0x33a0,0x33c0,0x33e0,
  0x3400,0x3420,0x3440,0x3460,0x3480,0x34a0,0x34c0,0x34e0,0x3500,0x3520,0x3540,0x3560,0x3580,0x35a0,0x35c0,0x35e0,
  0x3600,0x3620,0x3640,0x3660,0x3680,0x36a0,0x36c0,0x36e0,0x3700,0x3720,0x3740,0x3760,0x3780,0x37a0,0x37c0,0x37e0,
  0x3800,0x3820,0x3840,0x3860,0x3880,0x38a0,0x38c0,0x38e0,0x3900,0x3920,0x3940,0x3960,0x3980,0x39a0,0x39c0,0x39e0,
  0x3a00,0x3a20,0x3a40,0x3a60,0x3a80,0x3aa0,0x3ac0,0x3ae0,0x3b00,0x3b20,0x3b40,0x3b60,0x3b80,0x3ba0,0x3bc0,0x3be0,
  0x3c00,0x3c20,0x3c40,0x3c60,0x3c80,0x3ca0,0x3cc0,0x3ce0,0x3d00,0x3d20,0x3d40,0x3d60,0x3d80,0x3da0,0x3dc0,0x3de0,
  0x3e00,0x3e20,0x3e40,0x3e60,0x3e80,0x3ea0,0x3ec0,0x3ee0,0x3f00,0x3f20,0x3f40,0x3f60,0x3f80,0x3fa0,0x3fc0,0x3fe0,
#endif
};

u32 gaussLut[4];
u32 gammaLut[5];

//Functions Implementation
// ----------------------------------------------------------------------------


void dogltmCreateLuts ()
{
    u32 idx;

    for (idx = 0; idx < 4; idx++)
    {
        gaussLut[idx] = ispcLumaDnsLut[(idx<< 3) + 0]         |
                        (ispcLumaDnsLut[(idx<< 3) + 1] << 4 ) |
                        (ispcLumaDnsLut[(idx<< 3) + 2] << 8 ) |
                        (ispcLumaDnsLut[(idx<< 3) + 3] << 12) |
                        (ispcLumaDnsLut[(idx<< 3) + 4] << 16) |
                        (ispcLumaDnsLut[(idx<< 3) + 5] << 20) |
                        (ispcLumaDnsLut[(idx<< 3) + 6] << 24) |
                        (ispcLumaDnsLut[(idx<< 3) + 7] << 28);
    }

    gammaLut[0] = ispcGammaLut0_32[0]       | ispcGammaLut0_32[1] <<  8 |
                  ispcGammaLut0_32[2] << 16 | ispcGammaLut0_32[3] << 24;
    gammaLut[1] = ispcGammaLut0_32[4]       | ispcGammaLut0_32[5] <<  8 |
                  ispcGammaLut0_32[6] << 16 | ispcGammaLut0_32[7] << 24;
    gammaLut[2] = ispcGammaLut0_32[8]         | ispcGammaLut32_255[0] << 8 |
                  ispcGammaLut32_255[1] << 16 | ispcGammaLut32_255[2] << 24;
    gammaLut[3] = ispcGammaLut32_255[3]       | ispcGammaLut32_255[4] << 8 |
                  ispcGammaLut32_255[5] << 16 | ispcGammaLut32_255[6] << 24;
    gammaLut[4] = ispcGammaLut32_255[7]       | ispcGammaLut32_255[8] << 8;

}

void MonoPipeCreateParams ()
{
    dogltmCreateLuts ();
}