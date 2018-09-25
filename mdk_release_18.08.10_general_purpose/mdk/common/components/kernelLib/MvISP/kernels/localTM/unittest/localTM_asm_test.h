#ifndef _localTM_ASM_TEST_H_
#define _localTM_TEST_H_

extern unsigned int localTMCycleCount;
void localTM_asm_test(half** luma_in, unsigned char** bg8, half** output, half *curves, unsigned int width, unsigned int run_no);

#endif //_localTM_ASM_TEST_H_
