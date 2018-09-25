#ifndef __SIPP_DISP2DEPTH_EXEC_H__
#define __SIPP_DISP2DEPTH_EXEC_H__
//#iunclude "VectorTypes.h"

extern void flip_dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width);
extern void dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width);

extern void flip_mvcvDisp2depth(uint8_t *input, half8 *output,half *LUT, uint32_t width);
extern void mvcvDisp2depth(uint8_t *input, half8 *output,half *LUT, uint32_t width);

extern void flip_mvcvDisp2depth16bit(uint16_t *input, half8 *output,half *LUT, uint32_t width);
extern void mvcvDisp2depth16bit(uint16_t *input, half8 *output,half *LUT, uint32_t width);

#endif
