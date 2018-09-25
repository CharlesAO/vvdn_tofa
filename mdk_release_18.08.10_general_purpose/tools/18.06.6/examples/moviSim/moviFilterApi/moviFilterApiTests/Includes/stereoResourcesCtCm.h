#ifndef __STEREO_RESOURCES_CT_CM_H__
#define __STEREO_RESOURCES_CT_CM_H__

#include <stdint.h>

#define IMG_W_CT_CM 640
#define IMG_H_CT_CM  32

int CM_ALFA = 8;    // COMB_COST = α*AD + β*(CTC<<3)
int CM_BETA = 12;   //  where AD = absDiff of 2 pix values; CTC = census transform cost
int CM_THRESHOLD = 0x7f; // CT: max disparity
int CT_THRESHOLD = 0x0;  // CT: bit = 1 if(curPix > central + TH)
int RATIO_THRESHOLD = 0x56;

alignas(16) uint8_t iBufStereo[2][IMG_W_CT_CM*IMG_H_CT_CM];
alignas(16) uint16_t iBufFlat[2][IMG_W_CT_CM*IMG_H_CT_CM];
alignas(16) uint16_t iBufPack[2][IMG_W_CT_CM*IMG_H_CT_CM];

alignas(16) uint8_t oBuf0[IMG_W_CT_CM*IMG_H_CT_CM * 96]; //Debug data  (up to 96 Bytes/pix)
alignas(16) uint32_t oBuf1[IMG_W_CT_CM*IMG_H_CT_CM]; //Main output (up to  4 Bytes/pix)

extern uint16_t aggLut[];

#endif
