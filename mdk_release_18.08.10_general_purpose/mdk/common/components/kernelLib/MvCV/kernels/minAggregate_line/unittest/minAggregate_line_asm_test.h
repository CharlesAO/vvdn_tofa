#ifndef _MINAGGREGATE_LINE_ASM_TEST_H_
#define _MINAGGREGATE_LINE_TEST_H_

#ifndef __AGREGGATECOSTSGBM_CLAMP_ASM_TEST_H__
#define __AGREGGATECOSTSGBM_CLAMP_ASM_TEST_H__

#ifndef __AGREGGATECOSTSGBM32_CLAMP_ASM_TEST_H__
#define __AGREGGATECOSTSGBM32_CLAMP_ASM_TEST_H__

#ifndef __MINKERNEL32_ASM_TEST_H__
#define __MINKERNEL32_ASM_TEST_H__

#ifndef __MINKERNEL64_ASM_TEST_H__
#define __MINKERNEL64_ASM_TEST_H__

extern unsigned int mvcvMinAggregate_lineCycleCount;
//	 void minAggregate_line(u8* input1, u8* input2, u8* output, u32* penaltyP1, u32* penaltyP2, u32 width, u8 activateClamp, u8 direction)
void mvcvMinAggregate_line_asm_test(unsigned char *input1, unsigned char *input2,  unsigned char *output, unsigned int *penaltyP1,  unsigned int *penaltyP2, unsigned int width, unsigned char activateClamp, unsigned char direction, unsigned int disparities);


#endif //__AGREGGATECOSTSGBM_CLAMP_ASM_TEST_H__
#endif //__AGREGGATECOSTSGBM32_CLAMP_ASM_TEST_H__
#endif //__MINKernel32_ASM_TEST_H__
#endif //__MINKERNEL64_ASM_TEST_H__
#endif // _MINAggregate_line_TEST_H_
