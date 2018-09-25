#ifndef _SCALEFP16_ASM_TEST_H_
#define _SCALEFP16_ASM_TEST_H_

extern unsigned int scaleFp16CycleCount;
                    
void scaleFp16_asm_test(half** in, half** out, half scale, unsigned int width);

#endif //_SCALEFP16_ASM_TEST_H_
