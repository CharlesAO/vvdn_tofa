#ifndef _tile_sampleru8_ASM_TEST_H_
#define _tile_sampleru8_ASM_TEST_H_

extern unsigned int tile_sampleru8CycleCount;

void tile_sampleru8_asm_test(unsigned char *input_tile, unsigned char * output_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);

#endif //_tile_sampleru8_ASM_TEST_H_
