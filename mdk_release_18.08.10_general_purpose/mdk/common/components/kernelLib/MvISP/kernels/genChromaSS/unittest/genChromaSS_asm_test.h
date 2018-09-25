#ifndef _genChromaSS_ASM_TEST_H_
#define _genChromaSS_ASM_TEST_H_

extern unsigned int mvispGenChromaSSCycleCount;
void mvispGenChromaSS_asm_test(unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width);

#endif //_genChromaSS_ASM_TEST_H_
