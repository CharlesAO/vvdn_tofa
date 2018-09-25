#ifndef _CVTCOLORKERNELCHROMAYUV420TONV12_ASM_TEST_H_
#define _CVTCOLORKERNELCHROMAYUV420TONV12_ASM_TEST_H_

extern unsigned int cvtColorChromaYUV420ToNV12CycleCount;

void ChromaYUV420ToNV12_asm_test(unsigned char *inU, unsigned char *inV ,unsigned char *outUV , unsigned int width);

#endif //_CVTCOLORKERNELCHROMAYUV420TONV12_ASM_TEST_H_
