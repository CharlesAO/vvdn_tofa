#ifndef _tile_samplerRGB888_ASM_TEST_H_
#define _tile_samplerRGB888_ASM_TEST_H_

extern unsigned int tile_samplerRGB888CycleCount;

void tile_samplerRGB888_asm_test(unsigned char *input_tile, unsigned char * output_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);

#endif //_tile_samplerRGB888_ASM_TEST_H_
