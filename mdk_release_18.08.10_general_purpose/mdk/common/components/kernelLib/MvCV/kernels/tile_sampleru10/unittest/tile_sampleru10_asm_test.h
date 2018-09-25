#ifndef _tile_sampleru10_ASM_TEST_H_
#define _tile_sampleru10_ASM_TEST_H_

extern unsigned int tile_sampleru10CycleCount;

void tile_sampleru10_asm_test(unsigned short* input_tile, unsigned short* output_tile, float* xcoord, float* ycoord, unsigned int tile_width, unsigned int tile_height);

#endif //_tile_sampleru10_ASM_TEST_H_
