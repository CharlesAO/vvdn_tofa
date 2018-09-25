#ifndef _CVTCOLORKERNELCHROMAYUV444TONV12_ASM_TEST_H_
#define _CVTCOLORKERNELCHROMAYUV444TONV12_ASM_TEST_H_

extern unsigned int cvtColorChromaYUV444ToNV12CycleCount;

void ChromaYUV444ToNV12_asm_test(unsigned char *inU[2], unsigned char *inV[2] ,unsigned char *outUV , unsigned int width);

#endif //_CVTCOLORKERNELCHROMAYUV444TONV12_ASM_TEST_H_
