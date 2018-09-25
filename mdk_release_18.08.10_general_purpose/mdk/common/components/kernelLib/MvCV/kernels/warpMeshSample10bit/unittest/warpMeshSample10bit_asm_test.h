#ifndef _warpMeshSample10bit_ASM_TEST_H_
#define _warpMeshSample10bit_ASM_TEST_H_

extern unsigned int warpMeshSample10bitCycleCount;

void warpMeshSample10bit_asm_test(unsigned short **input_tile, unsigned short *output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);

#endif //_warpMeshSample10bit_ASM_TEST_H_
