#ifndef __STEREO_RESOURCES_CT_H__
#define __STEREO_RESOURCES_CT_H__

#include <stdint.h>

// Setup test 1: Solid-Color u8-input CT only various modes, u8 input
#define IMG_W  64
#define IMG_H   8

extern uint8_t  iBufA[IMG_W*IMG_H];
extern uint8_t  iBufB[IMG_W*IMG_H];
extern uint64_t oBufStereo[IMG_W*IMG_H];

extern uint32_t eBufA_PV0_CT32[IMG_W*IMG_H];
extern uint32_t eBufA_PV8_CT24[IMG_W*IMG_H];
extern uint64_t eBufA_PV0_CT64[IMG_W*IMG_H];
extern uint64_t eBufA_PV8_CT56[IMG_W*IMG_H];

extern uint32_t eBufB0_PV0_CT32[IMG_W*IMG_H];
extern uint64_t eBufB1_PV0_CT64[IMG_W*IMG_H];
extern uint32_t eBufB2_PV0_CT32[IMG_W*IMG_H];
#endif
