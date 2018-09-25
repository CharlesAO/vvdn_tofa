#ifndef _BILINEAR_INTERPOLATION_ASM_TEST_H_
#define _BILINEAR_INTERPOLATION_ASM_TEST_H_

#include <mv_types.h>

#define MAX_SIZE 5000

extern unsigned int bilinearInterpolationCycleCount;
void bilinearInterpolation_asm_test(u8 * line1,
                    u8 * line2,
                    u8 * dst,
                    u16 * precalcPos,
                    u16 * lut,
                    int stopW,
                    int godValue);

#endif //_BILINEAR_INTERPOLATION_U16_ASM_TEST_H_
