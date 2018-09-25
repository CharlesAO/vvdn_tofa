#ifndef __HAMMING_DISTANCE_ASM_TEST_H__
#define __HAMMING_DISTANCE_ASM_TEST_H__

#define MAX_DESCRIPTOR_SIZE    128
#define MAX_ARRAY_SIZE         512

extern unsigned int mvcvHammingDistanceCycleCount;
void mvcvHammingDistance_asm_test(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances);

#endif //__HAMMING_DISTANCE_ASM_TEST_H__
