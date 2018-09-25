///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    bilinearInterpolation_u16 kernel function call for unitary test
///

#include <bilinearInterpolation.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 5000
#define ALIGN(x) __attribute__((aligned(x)))

//#define UNIT_TEST_USE_C_VERSION

u8   line1[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
u8   line2[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) dst_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8  __attribute__((section(".kept.data"))) dst[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) dst_u32postPad[4] __attribute__ ((aligned (16)));

u16 ALIGN(8) precalcPos[10];
u16 ALIGN(8) lut[40];
int stopW;
int godValue;

int main(void)
{

    #ifdef UNIT_TEST_USE_C_VERSION
        bilinearInterpolation(
                    line1,
                    line2,
                    dst,
                    precalcPos,
                    lut,
                    stopW,
                    godValue);
    #else
        bilinearInterpolation_asm(
                    line1,
                    line2,
                    dst,
                    precalcPos,
                    lut,
                    stopW,
                    godValue);
    #endif


    SHAVE_HALT;
    return 0;
}
